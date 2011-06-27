#ifndef SRC_COMMON_MESSAGE_QUEUE_H
#define SRC_COMMON_MESSAGE_QUEUE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

class MessageQueue {
public:
    enum {MAXLEN = 256};

    /// message format
    struct Msg {
        long int type;
        char msg[MAXLEN];
        Msg(): type(1) {
            for (int i = 0; i< MAXLEN; ++i)
                msg[i] = 0;
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

#endif
