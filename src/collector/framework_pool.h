#ifndef SRC_COLLECTOR_FRAMEWORK_POOL_H
#define SRC_COLLECTOR_FRAMEWORK_POOL_H

class FrameworkPool {
public:
    typedef function<void(FrameworkPtr ptr)> FrameworkFunc;

public:
    void Insert(const FramworkPtr& ptr);
    
    bool Delete(const string& name);

    bool FindToDo(const string& name, FrameworkFunc func);
private:
    map<string, FramworkPtr> m_map;
};

#endif
