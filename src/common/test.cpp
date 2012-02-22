#include <map>
#include <string>
#include <iostream>
#include "register.h"
using std::string;
using std::cout;
using std::map;
using std::endl;
class Foo {
public:
    Foo() : m_value(10) {
        cout << "Foo Created" << endl;
    }
    int Value() {
        return m_value;
    }
private:
    int m_value;
};

REGISTER_CLASS(Foo);

int main () {
    return 0;
}
