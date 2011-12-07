#include "common/singleton.h"
#include <gtest/gtest.h>

struct Str {
    int n;
    int m;
};


TEST(NormalTest, All) {
    Str* ptr = Singleton<Str>::Instance();
    ptr->n = 10;
    ptr->m = 100;
    EXPECT_EQ((Singleton<Str>::Instance())->n, 10);
    EXPECT_EQ((Singleton<Str>::Instance())->m, 100);
    ptr->n = 9999;
    ptr->m = -1;
    EXPECT_EQ((Singleton<Str>::Instance())->n, 9999);
    EXPECT_EQ((Singleton<Str>::Instance())->m, -1);
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


