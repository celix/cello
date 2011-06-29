#include <iostream>
#include <string>

#if 0
#include "concurrency/ThreadManager.h"
#include "concurrency/PosixThreadFactory.h"
#include "protocol/TBinaryProtocol.h"
#include "server/TSimpleServer.h"
#include "server/TThreadPoolServer.h"
#include "server/TThreadedServer.h"
#include "transport/TServerSocket.h"
#include "transport/TTransportUtils.h"
#endif

#include "gen-cpp/Echo.h"

#include "common/rpc.h"

using std::string;
using std::cout;
using std::endl;

#if 0
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace boost;
#endif

class EchoHandler : public EchoIf {
public:
    int GetInt(int num) {
        cout << __func__ << endl;
        return num;
    }
    int GetIntAsync(int num) {
        cout << __func__ << endl;
        sleep(20);
        return 10;
    }
    void GetString(string& _return, const string& str) {
        cout << __func__ << endl;
        _return = str;
    }
    void GetInfo(Info& info) {
        info.id = 1;
        info.name = "chenjing";
        info.size = 10;
    }
};

int main() {
    Rpc<EchoHandler, EchoProcessor>::Listen(9999);
#if 0
    shared_ptr<EchoHandler> handler(new EchoHandler);
    shared_ptr<TProcessor> processor(new EchoProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    TSimpleServer server(processor, serverTransport, transportFactory,
                         protocolFactory);
    cout << "Starting the server" << endl;
    server.serve();
    cout << "Done" << endl;
#endif
    return 0;
}
