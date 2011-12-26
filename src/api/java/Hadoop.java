public class Hadoop {
    public static void main(String[] args) {
        TaskInfo info = new TaskInfo();
        info.cmd = "bash";
        info.arguments = "/home/chris/hadoop-0.20.2/bin/hadoop ";
        info.arguments += "--config /home/chris/hadoop-0.20.2/conf tasktracker";
        info.framework_name = "Hadoop";
        info.need_cpu = 0.5;
        info.need_memory  = 500;
        Proxy proxy = Cello.getProxy("10.5.0.176:9997");
        long id = proxy.submit(info);
        System.out.println("submit Hadoop tasktracker success.\nId is " + id);
    }
}
