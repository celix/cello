#include <lxc/lxc.h>
#include <lxc/conf.h>

int main() {
    char* args[2] = {"/bin/bash", "/home/chris/a.sh"};
    lxc_start("test", args, NULL);
    return 0;
}
