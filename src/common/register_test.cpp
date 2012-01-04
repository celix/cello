#include <gtest/gtest.h>
#include "common/register.h"
#include <iostream>

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

TEST(NormalTest, All) {
    Foo* ptr = static_cast<Foo*>(Class::NewInstance("Foo"));
    ASSERT_EQ(ptr->Value(), 10);
    delete ptr;
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

