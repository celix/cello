#include <iostream>
#if 0
#include "protocol/TBinaryProtocol.h"
#include "transport/TSocket.h"
#include "transport/TTransportUtils.h"
#endif
#include "./gen-cpp/Echo.h"
#if 0
using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace boost;
#endif
#include "common/rpc.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char** argv) {
    try {
        Proxy<EchoClient> proxy = Rpc<EchoClient, EchoClient>::GetProxy("10.5.0.174:9999", 10000);
        cout << proxy().GetInt(10) << endl;
        cout << proxy().GetIntAsync(10) << endl;
        string ss;
        proxy().GetString(ss, "test");
        cout << ss << endl;
    } catch (TException &tx) {
        printf("ERROR: %s\n", tx.what());
    }
    return 0;
}
