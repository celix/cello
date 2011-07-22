#include <sys/types.h>
#include <unistd.h>
#include "common/singleton.h"

#include <iostream>

using std::cout;
using std::endl;

class A {
public:
    int m;
};

class Test {
public:
    A* m_ptr;
    void Init() {
        m_ptr = new A;
        m_ptr->m = 10;
    }
};

typedef Singleton<Test> Mgr;

int main() {
    Mgr::Instance()->Init();
    pid_t pid = fork();
    if (pid != 0) {
        // parent process
        cout << "parent: " << Mgr::Instance()->m_ptr->m << endl;
    } else {
        // child
        cout << "child: " << Mgr::Instance()->m_ptr->m << endl;
    }
    return 0;
}
