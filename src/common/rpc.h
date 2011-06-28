#ifndef SRC_COMMON_RPC_H
#define SRC_COMMON_RPC_H

#include "concurrency/ThreadManager.h"
#include "concurrency/PosixThreadFactory.h"
#include "protocol/TBinaryProtocol.h"
#include "server/TSimpleServer.h"
#include "server/TThreadPoolServer.h"
#include "server/TThreadedServer.h"
#include "transport/TServerSocket.h"
#include "transport/TTransportUtils.h"

#include "glog/logging.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace boost;

template <typename T>
class Rpc {
public:
    static void Listen(int port) {
        shared_ptr<T> handler(new T);
        shared_ptr<TProcessor> processor(new EchoProcessor(handler));
        shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
        shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
        shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
        TSimpleServer server(processor, serverTransport, transportFactory,
                             protocolFactory);
        LOG(INFO) << "Starting the rpc server.";
        server.serve();
    }

    static T GetProxy(const string& ip, int port, int timeout
            shared_ptr<TTransport>& connection) {
        TSocket* sc = new TSocket(ip, port);
        sc->setRecvTimeout(timeout);
        shared_ptr<TTransport> socket(sc);
        shared_ptr<TTransport> transport(new TBufferedTransport(socket));
        shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
        EchoClient client(protocol);
        connection = transport;
        return client;
    }
};

#endif
