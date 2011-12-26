
interface ClientProtocol {
   public long submit(final TaskInfo info);

   public int query(long id, TaskInfo info);
   
   public int removeTask(long id);
}
