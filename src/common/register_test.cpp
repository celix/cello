#include <gtest/gtest.h>
#include <iostream>
#include "common/register.h"

using std::cout;
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

class B {
public:
    B() : m_value(100) {
        cout << "B created" << endl;
    }
    int Value() {
        return m_value;
    }
private:
    int m_value;
};
REGISTER_CLASS(B);

TEST(RegistTest, Normal) {
    B* b = static_cast<B*>(Singleton<Class>::Instance()->NewInstance("B"));
    ASSERT_EQ(b->Value(), 100);
    Foo* foo = static_cast<Foo*>(Singleton<Class>::Instance()->NewInstance("Foo"));
    ASSERT_EQ(foo->Value(), 10);
}

int main (int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

