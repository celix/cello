#ifndef SRC_COMMON_RPC_H
#define SRC_COMMON_RPC_H

#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <server/TThreadPoolServer.h>
#include <server/TThreadedServer.h>
#include <transport/TServerSocket.h>
#include <transport/TTransportUtils.h>
#include <transport/TSocket.h>

#include <glog/logging.h>
#include <string>

using std::string;

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace apache::thrift::concurrency;

//using namespace boost;
using boost::shared_ptr;

template <typename T, typename P>
class Rpc {
public:
    enum {DEFAULT_THREAD_NUM = 2};

    static void Listen(int port) {
        shared_ptr<T> handler(new T);
        shared_ptr<TProcessor> processor(new P(handler));
        shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
        shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
        shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
        shared_ptr<ThreadManager> threadManager =
            ThreadManager::newSimpleThreadManager(DEFAULT_THREAD_NUM);
        shared_ptr<PosixThreadFactory> thread_factory(new PosixThreadFactory());
        threadManager->threadFactory(thread_factory);
        threadManager->start();
        TThreadPoolServer server(processor,
                                 serverTransport,
                                 transportFactory,
                                 protocolFactory,
                                 threadManager);
        LOG(INFO) << "Starting the rpc server.";
        server.serve();
    }

    static T GetProxy(const string& endpoint, int timeout,
            shared_ptr<TTransport>* connection) {
        // split endpoint into ip and port
        string ip = endpoint.substr(0, endpoint.find(":"));
        string str_port = endpoint.substr(endpoint.find(":") + 1);
        int port = atoi(str_port.c_str());
        TSocket* sc = new TSocket(ip, port);
        sc->setRecvTimeout(timeout);
        shared_ptr<TTransport> socket(sc);
        shared_ptr<TTransport> transport(new TBufferedTransport(socket));
        shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
        T client(protocol);
        *connection = transport;
        return client;
    }
};

#endif
