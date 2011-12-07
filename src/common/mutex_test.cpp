#include "common/mutex.h"
#include <gtest/gtest.h>

using cello::Mutex;
using cello::MutexLocker;

TEST(TestLock, All) {
    Mutex mutex;
    mutex.Lock();
    mutex.Unlock();
}

TEST(TestLocker, All) {
    Mutex mutex;
    MutexLocker locker(mutex);
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
