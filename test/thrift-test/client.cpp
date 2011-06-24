#include <iostream>

#include "protocol/TBinaryProtocol.h"
#include "transport/TSocket.h"
#include "transport/TTransportUtils.h"

#include "./gen-cpp/Echo.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace boost;

int main(int argc, char** argv) {
    TSocket* sc = new TSocket("10.5.0.174", 9888);
    sc->setRecvTimeout(10000);
    shared_ptr<TTransport> socket(sc);
    shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    EchoClient client(protocol);

    try {
        transport->open();
        cout << client.GetInt(10) << endl;
        
        cout << client.GetIntAsync(10) << endl;
        string ss;
        client.GetString(ss, "test");
        cout << ss << endl;
        transport->close();
    } catch (TException &tx) {
        printf("ERROR: %s\n", tx.what());
    }
    return 0;
}
