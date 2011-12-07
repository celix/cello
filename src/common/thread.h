#ifndef SRC_COMMON_THREAD_H
#define SRC_COMMON_THREAD_H

#include <pthread.h>

#include <tr1/memory>
#include <tr1/functional>

using std::tr1::function;
using std::tr1::placeholders::_1;
using std::tr1::placeholders::_2;

namespace cello {

    class Thread {
        public:
            typedef function<void()> ThreadFunc;

        public:
            Thread(): m_entry(NULL),
            m_context(NULL),
            m_param(0),
            m_is_running(false) {}

            Thread(ThreadFunc entry, void* context = NULL, unsigned long long param = 0):
                m_entry(entry),
                m_context(context),
                m_param(param),
                m_is_running(false) {}

            virtual ~Thread() {}

            //void Init(ThreadFunc entry, void* context = NULL, unsigned long long param = 0);
            void DoStart();
            bool Start();
            bool Join();
            bool IsRunning() {
                return m_is_running;
            }
            void Terminate();

        private:
            static void* Entry(void* in_thread);
        private:
            pthread_t m_id;
            ThreadFunc m_entry;
            void* m_context;
            unsigned long long m_param;
            bool m_is_running;
    };

}
#endif
