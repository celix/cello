#ifndef SRC_COMMON_MESSAGE_QUEUE_H
#define SRC_COMMON_MESSAGE_QUEUE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <string>

using std::string;

namespace cello {

    class MessageQueue {
        public:
            enum {MAXLEN = 256};
            enum {QUEUE_TYPE = 1};

            /// message format
            struct Message {
                long int type;
                mutable char content[MAXLEN];

                Message() : type(QUEUE_TYPE) {
                    memset(content, 0, MAXLEN);
                }

                Message(const char* ptr) : type(QUEUE_TYPE) {
                    memset(content, 0, MAXLEN);
                    strncpy(content, ptr, strlen(ptr));
                }

                Message(const string& ss) : type(QUEUE_TYPE) {
                    memset(content, 0, MAXLEN);
                    strncpy(content, ss.c_str(), ss.size());
                }
                char* Get() const {
                    return content;
                }
            };

            explicit MessageQueue(key_t key);
            ~MessageQueue();

            /// send message 
            void Send(void* data);

            /// receive message
            void Receive(void* data);

        private:
            /// check the operation return value, if error then throw an exception
            void CheckError(const char* info, int code);

        private:
            key_t m_key;
            int m_id;
    };

}
#endif
