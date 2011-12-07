#include <gtest/gtest.h>
#include "common/pool.h"

void Func(int n) {
    --n;
}

TEST(SimpleTest, All) {
    Pool<int, int> int_pool;
    int_pool.Insert(1, 1);
    EXPECT_TRUE(int_pool.Find(1));
    EXPECT_FALSE(int_pool.Find(2));
    int_pool.Delete(1);
    EXPECT_FALSE(int_pool.Find(1));
}

TEST(FunctionTest, All) {
    Pool<int, int> pool;
    pool.Insert(1, 1);
    EXPECT_TRUE(pool.FindToDo(1, Func));
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

