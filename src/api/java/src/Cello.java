import org.apache.thrift.TException;
import org.apache.thrift.transport.TSSLTransportFactory;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TSSLTransportFactory.TSSLTransportParameters;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;

public class Cello {

    public static long Submit(final String address, final TaskInfo info) {
        String[] data = address.split(":");
        int port = Integer.parseInt(data[1]);
        long id = -1;
        try {
            TTransport transport;
            transport = new TSocket(data[0], port);
            transport.open();
            TProtocol protocol = new TBinaryProtocol(transport);
            Scheduler.Client client = new Scheduler.Client(protocol);
            id = client.Submit(info);
            transport.close();
        } catch (TException x) {
            x.printStackTrace();
        }
        return id;
    }
    
    public static int Query(final String address, int task_id, TaskInfo info) {
        String[] data = address.split(":");
        int port = Integer.parseInt(data[1]);
        int ret = -1;
        try {
            TTransport transport;
            transport = new TSocket(data[0], port);
            transport.open();
            TProtocol protocol = new TBinaryProtocol(transport);
            Scheduler.Client client = new Scheduler.Client(protocol);
            info = client.Query(task_id);
            ret = 0;
            transport.close();
        } catch (TException x) {
            x.printStackTrace();
        }
        return ret;
    }

    public static int RemoveTask(final String address, int task_id) {
        String[] data = address.split(":");
        int port = Integer.parseInt(data[1]);
        int ret = -1;
        try {
            TTransport transport;
            transport = new TSocket(data[0], port);
            transport.open();
            TProtocol protocol = new TBinaryProtocol(transport);
            Scheduler.Client client = new Scheduler.Client(protocol);
            ret = client.RemoveTask(task_id);
            transport.close();
        } catch (TException x) {
            x.printStackTrace();
        }
        return ret;
    }
}
