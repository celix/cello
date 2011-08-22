#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <time.h>

#include <vector>
#include <sstream>

#include "glog/logging.h"
#include "gflags/gflags.h"
#include "cellet/container.h"
#include "cellet/message_manager.h"
#include "cellet/system.h"
#include "common/string_utility.h"

using std::vector;
using std::stringstream;

DECLARE_string(work_directory);

Container::Container(const MessageQueue::Message& msg) : m_pid(0), m_c_args(0) {
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

int Container::Init() {
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
    LOG(INFO) << "Start Executor  ID:" << m_info.id;
    // child process
    m_pid = fork();
    if (m_pid == 0) {
        RedirectLog();
        // find cmd path automatically
        execvp(m_info.cmd.c_str(), m_c_args);
        LOG(ERROR) << "execute cmd error: " << m_info.cmd;
        exit(-1);
    } else {
        ContainerStarted();
    }
}

MessageQueue::Message Container::ToMessage() {
    char data[MessageQueue::MAXLEN] = {0};
    // convert executor information into a string with "\n" as separator
    snprintf(data, sizeof(data), "%lld\n%d\n", m_info.id, m_state);
    MessageQueue::Message msg(data);
    return msg;
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
