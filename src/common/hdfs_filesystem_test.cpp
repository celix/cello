#include <gtest/gtest.h>
#include "common/hdfs_filesystem.h"

TEST(NormalTest, Copy) {
    HdfsFileSytem hdfs;
    if (hdfs.Connect("default", 0) == 0) {
        const char* src = "/tmp/test.txt";
        const char* dest = "cello/test.txt";
        const char* dest1 = "/tmp/abc.txt";
        hdfs.CopyFromLocalToDfs(src, dest);
        hdfs.CopyFromDfsToLocal(dest, dest1);
    }
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


