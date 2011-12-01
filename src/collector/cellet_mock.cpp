#include <iostream>

#include "include/proxy.h"
#include "common/rpc.h"

using std::cout;
using std::endl;

int main() {
    shared_ptr<TTransport> transport;
    CollectorClient proxy = Rpc<CollectorClient, CollectorClient>::GetProxy("10.5.0.174:9998", 2000, &transport);
    try {
        transport->open();
        MachineInfo info;
        info.endpoint = "10.5.0.171:9997";
        info.avail_cpu = 2;
        info.avail_memory = 1000;
        proxy.Heartbeat(info);
        transport->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what();       
    }
    return 0;
}
