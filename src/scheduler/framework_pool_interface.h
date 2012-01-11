
#include <list>
#include <string>
#include <tr1/memory>
#include <tr1/functional>

#include "scheduler/framework.h"
#include "scheduler/task.h"
#include "common/rwlock.h"
#include "proxy/scheduler_wrapper.h"

using std::string;
using std::list;
using std::tr1::function;
using std::tr1::placeholders::_1;
using std::tr1::placeholders::_2;
using cello::RWLock;

class FrameworkPool {
public:
    virtual ~FrameworkPool() {}
    typedef function<void(Framework*)> FrameworkFunc;

    /// @brief: read information from xml file, and init all the framwork
    virtual int Init(const string& conf_file) = 0;
    
    /// add the task into correspond framework
    virtual void AddTask(const TaskPtr& task) = 0;
    
    /// @brief: remove task from its framework task queue
    virtual void RemoveTask(const TaskPtr& task);

    /// @brief: get executor information of framework
    virtual bool GetFrameworkExecutor(const string& name, TaskInfo* info);

    /// @brief: pick a task
    virtual TaskPtr GetTask();
    
    /// find the framework and do func on it
    /// find the framework return true, else return false
    virtual bool FindToDo(const string& name, FrameworkFunc func);

    virtual int AddFramework(const FrameworkInfoWrapper& info);

    virtual bool DeleteFramework(const string& name);

};

typedef Singleton<FrameworkPool> FrameworkMgr;

