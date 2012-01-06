#include "hdfs.h"
#include <assert.h>

#include <iostream>

using std::cout;
using std::endl;
int main () {
    hdfsFS fs = hdfsConnect("default", 0);
    if (!fs) {
        cout << "error" << endl;
        return -1;
    }
    const char* src = "/tmp/test.txt";
    const char* dest = "test2.txt";
    hdfsFS lfs = hdfsConnect(NULL, 0);
    if (!lfs) {
        cout << "error" << endl;
        return -1;
    }
    cout << hdfsCopy(lfs, src, fs, dest) << endl;
    hdfsDisconnect(fs);
    return 0;
}
