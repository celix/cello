#include "gflags/gflags.h"

DEFINE_int32(port, 9998, "cellet port");
DEFINE_string(work_directory, "/tmp/cello", "cellet work directory");
DEFINE_string(scheduler_endpoint, "10.5.0.170:9997", "scheduler endpoint");
DEFINE_string(collector_endpoint, "10.5.0.170:9998", "collector endpoint");
DEFINE_string(log, "/tmp/log/cellet", "cellet log path");

