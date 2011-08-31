#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <time.h>


#include <vector>
#include <sstream>

#include "glog/logging.h"
#include "gflags/gflags.h"
#include "lxc/lxc.h"
#include "lxc/conf.h"
#include "cellet/container.h"
#include "cellet/message_manager.h"
#include "cellet/system.h"
#include "common/string_utility.h"

using std::vector;
using std::stringstream;

DECLARE_string(work_directory);

Container::Container(const MessageQueue::Message& msg) : m_pid(0),
                                                         m_c_args(0),
                                                         m_start_time(0) {
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
            return -1;
        }
        LOG(INFO) << "create framework directory: " << framework_dir;
    }
    // create work directory
    char path[256] = {0};
    snprintf(path, sizeof(path), "%s/%lld", framework_dir.c_str(), m_info.id);
    if (mkdir(path, S_IRWXU|S_IRWXG|S_IROTH) < 0) {
        LOG(ERROR) << "create work directory error: " << path;
        return -1;
    }
    m_work_diectory = path;
    LOG(INFO) << "create work directory: " << path;
    // switch work directory
    if (chdir(path) < 0) {
        LOG(ERROR) << "change directory error: " << path;
        return -1;
    }
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

void Container::Execute() {
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
    if (m_pid == 0) {
        RedirectLog();
        // use lxc api to execute cmd
        lxc_conf* conf = lxc_conf_init();
        lxc_start(m_name.c_str(), m_c_args, conf);
        free(conf);
        LOG(INFO) << "execute cmd terminate: " << m_info.cmd;
        exit(-1);
    } else {
        SetResourceLimit();
        ContainerStarted();
    }
}

MessageQueue::Message Container::ToMessage() {
    char data[MessageQueue::MAXLEN] = {0};
    // convert executor information into a string with "\n" as separator
    snprintf(data, sizeof(data), "%lld\n%d\n", m_info.id, m_state);
    return data;
}

void Container::ContainerFinished() {
    LOG(INFO) << "Container Finished  ID:" << m_info.id << " PID:" << m_pid;
    // change status
    WriteLocker locker(m_lock);
    m_state = CONTAINER_FINISHED;
    MessageQueue::Message msg = ToMessage();
    MsgQueueMgr::Instance()->Get(EXECUTOR_STATE_KEY)->Send(&msg);
}

void Container::ContainerStarted() {
    LOG(INFO) << "Container Started ID:" << m_info.id << " PID:" << m_pid;
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

ExecutorStat Container::GetUsedResource() {
    ReadLocker locker(m_lock);
    // get cpu time
    char str_value[64] = {0};
    int len = 0;
    lxc_cgroup_get(m_name.c_str(), "cpuacct.usage", str_value, len);
    long long cpu_value = atoll(str_value);
    memset(str_value, 0, len);
    lxc_cgroup_get(m_name.c_str(), "cpuacct.usage", str_value, len);
    long long mem_value = atoll(str_value);
    double cpu_usage = 0;
    // change cpu time into cpu usage
    if (cpu_value > 0) {
        // change nano sec into sec
        cpu_value /= 1000 * 1000 * 1000;
        time_t cur_time = time(0);
        cpu_usage = cpu_value / (cur_time - m_start_time);
    }
        
    // change byte into mega byte
    if (mem_value > 0)
        mem_value = mem_value / (1024 * 1024);
    else
        mem_value = 0;
    ExecutorStat es(m_info.framework_name, cpu_usage, mem_value);
    return es;
}

void Container::SetResourceLimit() {
    // set memory
    char data[32] = {0};
    snprintf(data, sizeof(data), "%d", m_info.need_memory * 1024 * 1024);
    lxc_cgroup_set(m_name.c_str(), "memory.limit_in_bytes", data);
    memset(data, 0, sizeof(data));
    // set cpu
    snprintf(data, sizeof(data), "%d",
             static_cast<int>(m_info.need_cpu * 1024 / DEFAULT_CPU_SHARE));
    lxc_cgroup_set(m_name.c_str(), "cpu.shares", data);
}
