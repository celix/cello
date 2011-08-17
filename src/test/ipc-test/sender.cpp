#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#include "info.h"

using std::cout;
using std::endl;

int main() {
    int id = msgget(KEY, 0666|IPC_CREAT);
    if (id < 0) {
        cout << "msgget failed with error: " << errno << endl;
        return -1;
    }
    Info data;
    snprintf(data.ss, sizeof(data.ss), "send information");
    if (msgsnd(id, (void* )&data, MAX, 0) < 0) {
        cout << "msgsnd failed with error: " << errno << endl;
        return -1;
    }
    while(1)
        sleep(1);
    return 0;
}
