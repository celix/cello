#include <gtest/gtest.h>
#include "cellet/system.h"
#include "cellet/container.h"

TEST(TestContainer, All) {
    Container a;
    const char* ss = "user 12\nsystem 4";
    EXPECT_EQ(16, static_cast<int>(a.ParseTime(ss)));
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


