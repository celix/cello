#include "gtest/gtest.h"
#include "get_ip.h"
#include <iostream>

using std::cout;
using std::endl;

TEST(TestGetIP, All) {
    string ip = GetIP();
    cout << ip << endl;
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
