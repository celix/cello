#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <string>
#include <algorithm>
#include <glog/logging.h>
#include <sys/param.h>
#include <dirent.h>

#include "cellet/system.h"

using std::string;

// @return:logic cpu numbers
int System::CpuNum() {
    int cpu_num = 0;

    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        return 1;
    }

    char buf[256] = {0};
    //  Look at each line, count line begin with
    while (fgets(buf, sizeof(buf)-1, fp)) {
        if (strncmp(buf, "processor", 9) == 0) {
            cpu_num++;
        }
    }

    // Done reading; close the file
    fclose(fp);
    return cpu_num ? cpu_num : 1;
}

// @return: total physical memory size
int System::TotalMemory() {
    int total = 0;
    char buf[1024];
    FILE* fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        return -1;
    }

    while (fgets(buf, sizeof(buf)-1, fp)) {
        if (strncmp(buf, "MemTotal:", 9))
            continue;
        char* b = strstr(buf, "MemTotal: ");
        sscanf(b, "MemTotal: %d", &total);
    }
    fclose(fp);
    total /= 1024;
    return total;
}



//  @return: used memory
int System::UsedMemory() {
    int total = TotalMemory();
    int free = PhysicalMemory();
    if (total == -1 || free == -1 || free > total)
        return -1;
    return total - free;
}

// @return: free physical memory
int System::PhysicalMemory() {
    int free = 0;
    char buf[1024];
    FILE* fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        return -1;
    }

    while (fgets(buf, sizeof(buf)-1, fp)) {
        if (strncmp(buf, "MemFree:", 8))
            continue;
        char* b = strstr(buf, "MemFree: ");
        sscanf(b, "MemFree: %d", &free);
    }
    fclose(fp);
    free /= 1024;
    return free;
}


// @return: return free swap size if success, or return -1
int System::VirtualMemory() {
    int free_swap = -1;
    // gather info from /proc/meminfo to get free swap
    FILE *fp = fopen("/proc/meminfo", "r");
    char buf[256] = {0};
    if (!fp) {
        return -1;
    }

    /* Look at each line, count line begin with  */
    while (fgets(buf, sizeof(buf)-1, fp)) {
        if (strncmp(buf, "SwapFree", 8) == 0) {
            if (sscanf(buf, "SwapFree: %d", &free_swap) < 1) {
                fclose(fp);
                return -1;
            }
            break;
        }
    }

    // Done reading; close the file
    fclose(fp);
    free_swap /= 1024;
    return free_swap;
}

// @return:return cpu load if success, or return -1.0
double System::LoadAvg() {
    double short_avg, medium_avg, long_avg;
    FILE* fp = fopen("/proc/loadavg", "r");
    if (!fp)
        return -1.0;

    // gather info from /proc/meminfo to get free swap
    if (!fp) {
        return -1.0;
    }

    fscanf(fp, "%lf %lf %lf", &short_avg, &medium_avg, &long_avg);
    fclose(fp);
    return medium_avg;
}

// @return: cpu usage
double System::CpuUsage() {
    FILE *fp;
    char line[8192];
    static uint64_t prev_used = 0;
    static uint64_t prev_total = 0;
    static bool first = true;

    if ((fp = fopen("/proc/stat", "r")) == NULL) {
        return -1.0;
    }

    uint64_t cpu_user = 0;
    uint64_t cpu_nice = 0;
    uint64_t cpu_sys = 0;
    uint64_t cpu_idle = 0;
    uint64_t cpu_iowait = 0;
    uint64_t cpu_hardirq = 0;
    uint64_t cpu_softirq = 0;
    uint64_t used = 0;
    uint64_t total = 0;
    while (fgets(line, 8192, fp) != NULL) {
        if (!strncmp(line, "cpu ", 4)) {
            sscanf(line + 5, "%llu %llu %llu %llu %llu %llu %llu",
                    &cpu_user, &cpu_nice, &cpu_sys, &cpu_idle,
                    &cpu_iowait, &cpu_hardirq, &cpu_softirq);
            used = cpu_user + cpu_nice + cpu_sys +
                   cpu_iowait + cpu_hardirq + cpu_softirq;
            total = used + cpu_idle;
            break;
        }
    }

    if (first) {
        first = false;
        prev_used = used;
        prev_total = total;
        fclose(fp);
        return 0.0;
    }

    // 回避除以0的问题
    if (total == prev_total) {
        total = prev_total + 1;
    }
    double cpu_usage = static_cast<double>(used - prev_used)
                        / static_cast<double>(total - prev_total);
    prev_used = used;
    prev_total = total;
    fclose(fp);
    return cpu_usage;
}


// @return: cpu usage
uint64_t System::CpuTime() {
    FILE *fp;
    char line[8192];
    if ((fp = fopen("/proc/stat", "r")) == NULL) {
        return -1;
    }

    uint64_t cpu_user = 0;
    uint64_t cpu_nice = 0;
    uint64_t cpu_sys = 0;
    uint64_t cpu_idle = 0;
    uint64_t cpu_iowait = 0;
    uint64_t cpu_hardirq = 0;
    uint64_t cpu_softirq = 0;
    uint64_t total = 0;
    while (fgets(line, 8192, fp) != NULL) {
        if (!strncmp(line, "cpu ", 4)) {
            sscanf(line + 5, "%llu %llu %llu %llu %llu %llu %llu",
                    &cpu_user, &cpu_nice, &cpu_sys, &cpu_idle,
                    &cpu_iowait, &cpu_hardirq, &cpu_softirq);
            total = cpu_user + cpu_nice + cpu_sys +
                   cpu_iowait + cpu_hardirq + cpu_softirq + cpu_idle;
            break;
        }
    }

    fclose(fp);
    return total;
}

void System::RemoveDir(const char* path) {
    DIR* dp = opendir(path);
    if (dp) {
        dirent* ep = NULL;
        while((ep = readdir(dp)) != NULL) {
            // get rid of .. and .
            if (strcmp("..", ep->d_name) != 0 &&
                strcmp(".", ep->d_name) != 0) {
                char file[256] = {0};
                snprintf(file, sizeof(file), "%s/%s", path, ep->d_name);
                struct stat info;
                stat(file, &info);
                // directory or file
                if (S_ISDIR(info.st_mode))
                    RemoveDir(file);
                else
                    unlink(file);
            }
        }
        rmdir(path);
    }
    closedir(dp);
}

void System::GetCurrentTime(char* time_str, int len) {
    time_t cur_time;
    time(&cur_time);
    tm* p = gmtime(&cur_time);
    snprintf(time_str, len, "%d%d%d-%d%d%d", (1900 + p->tm_year),
             (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
}
