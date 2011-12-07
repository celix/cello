#include <gtest/gtest.h>
#include "common/get_ip.h"

TEST(TestGetIP, All) {
    string ip = GetIP();
    EXPECT_EQ("10.5.0.174", ip);
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
