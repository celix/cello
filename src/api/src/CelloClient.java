import org.apache.thrift.TException;
import org.apache.thrift.transport.TSSLTransportFactory;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TSSLTransportFactory.TSSLTransportParameters;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;

public class CelloClient {
    public static void main(String [] args) {
        try {
            TTransport transport;
            transport = new TSocket("10.5.0.174", 9999);
            transport.open();
            TProtocol protocol = new TBinaryProtocol(transport);
            Scheduler.Client client = new Scheduler.Client(protocol);
            perform(client);
            transport.close();
        } catch (TException x) {
            x.printStackTrace();
        } 
    }

    private static void perform(Scheduler.Client client) throws TException
    {
        TaskInfo task_info = new TaskInfo();
        task_info.framework_name = "Binos";
        task_info.cmd = "ls";
        long id = client.Submit(task_info);
        System.out.println(id);
    }
}
