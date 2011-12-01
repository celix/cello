#include "gflags/gflags.h"

DEFINE_string(file, "", "framework configuration file");
DEFINE_string(name, "", "framework name");

int main(int argc, char ** argv) {
    // set up flags
    if (argc == 3) {
        google::ParseCommandLineFlags(&argc, &argv, true);
    } else {
        return -1;
    }
    
    return 0;
}
