#ifndef SRC_COMMON_THREAD_H
#define SRC_COMMON_THREAD_H

#include <pthread.h>

#include <tr1/memory>
#include <tr1/functional>

using std::tr1::function;
using std::tr1::placeholders::_1;
using std::tr1::placeholders::_2;

class Thread {
public:
    typedef function<void*(void* context, unsigned long long param)> ThreadFunc;

public:
    Thread(): m_entry(NULL),
              m_context(NULL),
              m_param(0),
              m_is_running(false) {}
    
    Thread(ThreadFunc entry, void* context = NULL, unsigned long long param = 0):
        m_entry(entry) {}

    virtual ~Thread() {}
   
    void Init(ThreadFunc entry, void* context = NULL; unsigned long long param = 0);
    bool Start();
    bool Join();
    bool IsRunning() {
        return m_is_running;
    }
    
private:
    static void Entry(void* unused);
private:
    pthread_t m_id;
    bool m_is_running;
    ThreadFunc m_entry;
    void* m_context;
    unsigned long long m_param;
};

#endif
