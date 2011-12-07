#include "cellet/system.h"
#include <gtest/gtest.h>

TEST(TestSystem, All) {
    System::RemoveDir("/tmp/cello");
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

