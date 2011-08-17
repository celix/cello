#include <iostream>
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
    if (msgrcv(id, (void* )&data, MAX, 0, 0) < 0) {
        cout << "msgrcv failed with error: " << errno << endl;
        return -1;
    }
    cout << data.ss << endl;
    return 0;
}

