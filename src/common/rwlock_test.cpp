#include "common/rwlock.h"
#include <gtest/gtest.h>

using cello::RWLock;
using cello::ReadLocker;
using cello::WriteLocker;

TEST(TestLock, ALl) {
    RWLock lock;
    lock.ReadLock();
    lock.Unlock();
    lock.WriteLock();
    lock.Unlock();
    lock.Unlock();
}

TEST(TestRead, All) {
    RWLock lock;
    ReadLocker locker(lock);
}

TEST(TestWrite, ALl) {
    RWLock lock;
    WriteLocker locker(lock);
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

