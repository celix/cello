import org.apache.thrift.transport.TSSLTransportFactory;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TSSLTransportFactory.TSSLTransportParameters;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;

public class Cello {
    public static Proxy GetProxy(final String address) {
        String[] data = address.split(":");
        int port = Integer.parseInt(data[1]);
        TTransport transport;
        transport = new TSocket(data[0], port);
        TProtocol protocol = new TBinaryProtocol(transport);
        Scheduler.Client client = new Scheduler.Client(protocol);
        return new Proxy(transport, client);
    }
}
