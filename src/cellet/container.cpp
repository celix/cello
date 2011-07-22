#include <sys/stat.h>
#include <sys/unistd.h>

#include <vector>

#include "glog/logging.h"
#include "cellet/container.h"
#include "common/string_utility.h"

using std::vector;

Container::Container(const MessageQueue::Message& msg) : m_pid(0) {
    vector<string> res;
    StringUtility::Split(msg.content, '\n', &res);
    int64_t id = atol(res[0].c_str());
    double need_cpu = atof(res[3].c_str());
    int need_memory = atoi(res[4].c_str());
    // set task information
    m_info.id = id;
    m_info.cmd = res[1];
    m_info.arguments = res[2];
    m_info.framework_name = res[3];
    m_info.need_cpu = need_cpu;
    m_info.need_memory = need_memory;
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
    snprintf(path, sizeof(path), "%s/%ld", framework_dir.c_str(), m_info.id);
    if (mkdir(path, S_IRWXU|S_IRWXG|S_IROTH) < 0) {
        LOG(ERROR) << "create work directory error: " << path;
        return -1;
    }
    LOG(INFO) << "create work directory: " << path;
    // switch work directory
    if (chdir(path) < 0) {
        LOG(ERROR) << "change directory error: " << path;
        return -1;
    }
    return 0;
}

void Container::Execute() {
    vector<string> args;
    StringUtility::Split(m_info.arguments, ' ', &args);
    // get cmd name;
    char name[30] = {0};
    strncpy(name, m_info.cmd.c_str(), m_info.cmd.length());
    // create arg list
    char* c_args[args.size() + 1] = {0};
    // add cmd name as argv[0];
    c_args[0] = name;
    // get the arguments
    for (int i = 0; i < args.size(); ++i)
        c_args[i + 1] = args[i].c_str();
    LOG(INFO) << "command argument list:";
    for (int i = 0; i < args.size() + 1; ++i)
        LOG(INFO) << c_args[i];
    // child process
    m_pid = fork();
    if (m_pid == 0) {
        // find cmd path automatically
        execvp(m_info.cmd.c_str(), c_args);
    }
}
