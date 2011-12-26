import org.apache.thrift.TException;
import org.apache.thrift.transport.TSSLTransportFactory;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TSSLTransportFactory.TSSLTransportParameters;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;

public class Proxy implements ClientProtocol {
    private TTransport mTransport;
    private Scheduler.Client mClient;

    public Proxy(TTransport tranport, Scheduler.Client client) {
        mTransport = tranport;
        mClient = client;
    }

    // @override
    public long submit(final TaskInfo info) {
        long id = -1;
        try {
            mTransport.open();
            id =  mClient.Submit(info);
            mTransport.close();
        } catch (TException x) {
            x.printStackTrace();
        }
        return id;
    }

    // @override
    public int query(long id, TaskInfo info) {
        int ret = -1;
        try {
            mTransport.open();
            info =  mClient.Query(id);
            ret = 0;
            mTransport.close();
        } catch (TException x) {
            x.printStackTrace();
        }
        return ret;
    }

    /// @override
    public int removeTask(long id) {
        int ret = -1;
        try {
            mTransport.open();
            ret =  mClient.RemoveTask(id);
            mTransport.close();
        } catch (TException x) {
            x.printStackTrace();
        }
        return ret;
    }
}

