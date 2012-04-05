#include <sys/stat.h>
#include <dirent.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <assert.h>

#include <vector>
#include <sstream>

#include <glog/logging.h>
#include <gflags/gflags.h>
#include <lxc/lxc.h>
#include <lxc/conf.h>
#include "cellet/container.h"
#include "cellet/message_manager.h"
#include "cellet/system.h"
#include "cellet/resource_manager.h"
#include "common/string_utility.h"
#include "common/filesystem.h"
#include "common/register.h"
#include "common/policy.h"

using std::vector;
using std::stringstream;
using cello::ReadLocker;
using cello::WriteLocker;

DECLARE_string(work_directory);
DECLARE_string(dfs_ip);
DECLARE_int32(dfs_port);

Container::Container(const MessageQueue::Message& msg) : m_pid(0),
                                                         m_c_args(0),
                                                         m_first(true),
                                                         m_prev_cpu(0.0),
                                                         m_prev_total(0.0) {
    vector<string> res;
    StringUtility::Split(msg.Get(), '\n', &res);
    int64_t id = atol(res[0].c_str());
    double need_cpu = atof(res[4].c_str());
    int need_memory = atoi(res[5].c_str());
    // set task information
    m_info.id = id;
    m_info.cmd = res[1];
    m_info.arguments = res[2];
    m_info.framework_name = res[3];
    m_info.need_cpu = need_cpu;
    m_info.need_memory = need_memory;
    m_state = CONTAINER_INIT;
}

Container::~Container() {
    // if have args then free args memory after container finished
    if (m_c_args)
        StringUtility::DestoryArgArray(m_c_args);
    // clear the work directory
    if(!m_work_diectory.empty())
        System::RemoveDir(m_work_diectory.c_str());
}

void Container::SetName() {
    char data[50] = {0};
    snprintf(data, sizeof(data), "%s_%lld", m_info.framework_name.c_str(),
             m_info.id);
    m_name = data;
}

int Container::Init() {
    SetName();
    string framework_dir = FLAGS_work_directory + "/";
    framework_dir += m_info.framework_name;
    // framework directory doesn't exist then create it
    if (access(framework_dir.c_str(), F_OK) < 0) {
        if (mkdir(framework_dir.c_str(), S_IRWXU|S_IRWXG|S_IROTH) < 0) {
            LOG(ERROR) << "create framework directory error: " << framework_dir;
            //return -1;
        }
        LOG(INFO) << "create framework directory: " << framework_dir;
    }
    // create work directory
    char path[256] = {0};
    snprintf(path, sizeof(path), "%s/%lld", framework_dir.c_str(), m_info.id);
    if (mkdir(path, S_IRWXU|S_IRWXG|S_IROTH) < 0) {
        LOG(ERROR) << "create work directory error: " << path;
        //return -1;
    }
    m_work_diectory = path;
    LOG(INFO) << "create work directory: " << path;
    if (PolicyMgr::Instance()->Get("PoolSchema") == "StandardPool") {
        // get files from dfs to work directory if there is file to download
        if (FetchFiles() < 0)
            return -1;
    }
    // switch work directory
    if (chdir(path) < 0) {
        LOG(ERROR) << "change directory error: " << path;
        return -1;
    }
    return 0;
}

int Container::FetchFiles() {
    vector<string> vt;
    StringUtility::Split(m_info.transfer_files, ' ', &vt);
    // get the filesystem handler
    FileSystem* fs_handler =
        static_cast<FileSystem*>(ClassInstance->NewInstance(PolicyMgr::Instance()->Get("FileSystem")));
    fs_handler->Connect(FLAGS_dfs_ip, FLAGS_dfs_port);
    for(vector<string>::iterator it = vt.begin(); it != vt.end(); ++it) {
        // copy file to work directory
        vector<string> tmp;
        StringUtility::Split(*it, '/', &tmp);
        string new_path = m_work_diectory + "/" + tmp[tmp.size() - 1];
        if (fs_handler->CopyFromDfsToLocal(*it, new_path) < 0) {
            LOG(ERROR) << "copy file failed: " << *it << " to " << new_path;
            fs_handler->Disconnect();
            delete fs_handler;
            return -1;
        }
        LOG(INFO) << "copy file success " << *it << " to " << new_path;
    }
    fs_handler->Disconnect();
    delete fs_handler;
    return 0;
}

void Container::RedirectLog() {
    // create log name and path
    char timestamp[20] = {0};
    System::GetCurrentTime(timestamp, sizeof(timestamp));
    char log_name[100] = {0};
    snprintf(log_name, sizeof(log_name), "%s_%lld_%s",
            m_info.framework_name.c_str(), m_info.id, timestamp);
    char log_path[200] = {0};
    snprintf(log_path, sizeof(log_path), "%s/%s", m_work_diectory.c_str(),
            log_name);
    DLOG(WARNING) << log_path;
    // open the log file redirect stdout and stderr
    int fd = open(log_path, (O_RDWR | O_CREAT), 0644);
    dup2(fd, 1);
    dup2(1, 2);
    close(fd);
}

void Container::CloseInheritedFD() {
#if 0
    char path[30] = {0};
    // get proc directory path
    snprintf(path, sizeof(path), "/proc/%d/fd", getpid());
    DIR* dp = opendir(path);
    if (dp) {
        dirent* ep = NULL;
        while ((ep = readdir(dp))) {
            int fd = atoi(ep->d_name);
            // get rid of stdin, stdout, stderr which is 0,1,2
            if (fd > 2)
                close(fd);
        }
    }
    closedir(dp);
#endif
    // close all the possible fd in linux
    for (int i = 3; i < 256; ++i)
        close(i);
}

int Container::Execute() {
    vector<string> args;
    StringUtility::Split(m_info.arguments, ' ', &args);
    // add cmd as argv[0]
    args.insert(args.begin(), m_info.cmd);
    // convert string vector to string array
    m_c_args = StringUtility::CreateArgArray(args);
    LOG(WARNING) << "command argument list:";
    stringstream oss;
    for (int i = 0; m_c_args[i]; ++i)
        oss << m_c_args[i] << " ";
    LOG(WARNING) << oss.str();
    LOG(INFO) << "Start Executor ID:" << m_info.id << " in container:" << m_name;
    // child process
    m_pid = fork();
    int ret = 0;
    if (m_pid == 0) {
        LOG(WARNING) << "in child process";
        // close all the fd inherited from parent
        CloseInheritedFD();
        RedirectLog();
        // use lxc api to execute cmd
        lxc_conf* conf = lxc_conf_init();
        ret = lxc_start(m_name.c_str(), m_c_args, conf);
        free(conf);
        if (ret >= 0) {
            exit(0);
        } else {
            fprintf(stderr, "%dexecute cmd terminate: %s\n", ret, m_info.cmd.c_str());
            exit(-1);
        }
    } else {
        sleep(1);
        ContainerStarted();
        return ret;
    }
}

MessageQueue::Message Container::ToMessage() {
    char data[MessageQueue::MAXLEN] = {0};
    // convert executor information into a string with "\n" as separator
    snprintf(data, sizeof(data), "%lld\n%d\n", m_info.id, m_state);
    return data;
}

void Container::ContainerFinished() {
    LOG(WARNING) << "Container Finished  ID:" << m_info.id << " PID:" << m_pid;
    // change status
    WriteLocker locker(m_lock);
    m_state = CONTAINER_FINISHED;
    MessageQueue::Message msg = ToMessage();
    MsgQueueMgr::Instance()->Get(EXECUTOR_STATE_KEY)->Send(&msg);
}

void Container::ContainerStarted() {
    LOG(INFO) << "Container Started ID:" << m_info.id << " PID:" << m_pid;
    // set resource limit
    SetResourceLimit();
    // change status
    WriteLocker locker(m_lock);
    m_state = CONTAINER_STARTED;
    // report the message
    MessageQueue::Message msg = ToMessage();
    MsgQueueMgr::Instance()->Get(EXECUTOR_STATE_KEY)->Send(&msg);
}

ContainerState Container::GetState() {
    ReadLocker locker(m_lock);
    return m_state;
}

ExecutorStatWrapper Container::GetUsedResource(double used_cpu, int used_memory) {
    ReadLocker locker(m_lock);
    int memory_usage = GetMemory();
    double cpu_usage = GetCpuUsage(used_cpu);
    // running tasks in executor
    int task_num = GetChildrenNum();
    ExecutorStatWrapper es(m_info.framework_name, m_info.id, cpu_usage, memory_usage, task_num);
    return es;
}

int Container::GetMemory() {
    char str_value[64] = {0};
    int len = 100;
    // get memory
    lxc_cgroup_get(m_name.c_str(), "memory.usage_in_bytes", str_value, len);
    uint64_t value = atoll(str_value);
    DLOG(WARNING) << "USED MEMORY:" << value;
    int memory;
    // change byte into mega byte
    if (value > 0)
        memory = value / (1024 * 1024);
    else
        memory = 0;
    LOG(INFO) << "framework:" << m_info.framework_name
              << " used memory:" << memory << "M";
    return memory;
}

double Container::GetCpuUsage(double used_cpu) {
    // get cpu time
    char str_value[256] = {0};
    lxc_cgroup_get(m_name.c_str(), "cpuacct.stat", str_value, sizeof(str_value));
    // USER_HZ unit
    uint64_t cur_cpu = ParseTime(str_value);
    double cpu_usage = 0;
    // change cpu time into cpu usage
    if (cur_cpu > 0) {
        if (m_first) {
            m_first = false;
            m_prev_cpu = cur_cpu;
            m_prev_total = System::CpuTime();
            DLOG(WARNING) << "CURRENT CPU : " << m_prev_cpu
                          << "CURRENT TOTAL: " << m_prev_total;
            return 0.0;
        }
        uint64_t cur_total = System::CpuTime();
        cpu_usage = static_cast<double>
                    (cur_cpu - m_prev_cpu) / (cur_total - m_prev_total);
        // we need to divide an modulus of the quota of the cpu divide all used cpu quota
        LOG(WARNING) << "true cpu usage :" << cpu_usage;
        cpu_usage /= m_info.need_cpu / used_cpu;
        LOG(WARNING) << "corrected cpu usage:" << cpu_usage;
        m_prev_cpu = cur_cpu;
        m_prev_total = cur_total;
        DLOG(WARNING) << "CURRENT CPU : " << m_prev_cpu
                      << "CURRENT TOTAL: " << m_prev_total;
    } else {
        DLOG(ERROR) << "Get cpu time error";
        cpu_usage = 0.0;
    }
    LOG(INFO) << "framework:" << m_info.framework_name
              << " used cpu usage:" << cpu_usage;
    return cpu_usage;
}

void Container::SetResourceLimit() {
    // set memory
    char data[64] = {0};
    snprintf(data, sizeof(data), "%lld", (long long int)m_info.need_memory * 1024 * 1024);
    lxc_cgroup_set(m_name.c_str(), "memory.limit_in_bytes", data);
    memset(data, 0, sizeof(data));
    // set cpu
    snprintf(data, sizeof(data), "%d",
             static_cast<int>(m_info.need_cpu * 512 / DEFAULT_CPU_SHARE));
    lxc_cgroup_set(m_name.c_str(), "cpu.shares", data);
}

uint64_t Container::ParseTime(const char* str) {
    uint64_t user, system;
    sscanf(str, "user %llu\nsystem %llu", &user, &system);
    return user + system;
}

int Container::GetChildrenNum() {
    char* res = (char* )malloc(1024);
    memset(res, 0, 1024);
    if (Shell(res, "lxc-ps -n %s", m_name.c_str()) < 0)
        return -1;
    vector<string> vt;
    StringUtility::Split(res, '\n', &vt);
    free(res);
    LOG(INFO) << "child process number: " << vt.size() - 2;
    // get rid of title and parent process
    return vt.size() - 2;
}

int Container::Shell(char* result, const char* format, ...) {
    char *cmd;
    FILE *f;
    int ret;
    va_list args;
    va_start(args, format);
    if (vasprintf(&cmd, format, args) == -1)
        return -1;
    if ((f = popen(cmd, "r")) == NULL)
        return -1;
    char ch;
    int i = 0;
    // get command execute result
    while ((ch = fgetc(f)) != EOF) {
        if (i < 1024)
            result[i++] = ch;
        else
            break;
    }
    ret = pclose(f);
    if (ret == -1)
        LOG(INFO) << "pclose error: " << strerror(errno);
    free(cmd);
    va_end(args);
    return ret;
}

int Container::Recycle() {
    // first stop the container
    if (lxc_stop(m_name.c_str()) >= 0) {
        LOG(INFO) << "stop container succeed. container name: " << m_name;
        // kill the process running starting the container
        if (kill(m_pid, SIGKILL) >= 0) {
            LOG(INFO) << "kill process succeed. pid: " << m_pid;
            return 0;
        } else {
            LOG(ERROR) << "kill process failed. pid: " << m_pid;
            return -2;
        }
    } else {
        LOG(ERROR) << "stop container failed. conatiner name: " << m_name;
        return -1;
    }
}
