//classe que estende Thread e implementa a tarefa de cada thread do programa 
class T extends Thread {
   //identificador da thread
   int id;
   //objeto compartilhado com outras threads
   S s;
  
   //construtor
   public T(int tid, S s) { 
      this.id = tid; 
      this.s = s;
   }

   //metodo main da thread
   public void run() {
      System.out.println("Thread " + this.id + " iniciou!");
      for (int i=0; i<10000000; i++) {
         this.s.inc();  
      }
      System.out.println("Thread " + this.id + " terminou!"); 
   }
}