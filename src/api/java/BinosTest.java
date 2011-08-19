

public class BinosTest {
    public static void main(String[] args) {
        TaskInfo info = new TaskInfo();
        info.cmd = "/bin/sh";
        info.arguments = "/home/chris/Master-Slave/bin/slaves.sh --port=8888";
        info.framework_name = "Binos";
        info.need_cpu = 0.5;
        info.need_memory  = 600;
        String address = "10.5.0.174:9997";
        long id = Cello.Submit(address, info);
        System.out.println("submit success.task id is " + id);
    }
}
