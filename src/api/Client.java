public class Client {
    public static void main(String[] args) {
        TaskInfo info = new TaskInfo();
        info.cmd = "/bin/ls";
        info.framework_name = "Binos";
        String address = "10.5.0.174:9999";
        long id = Cello.Submit(address, info);
        System.out.println("submit success.task id is " + id);
    }
}
