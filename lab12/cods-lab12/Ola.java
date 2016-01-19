/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 12 */
/* Codigo: Estendendo a classe Thread de Java */


//--PASSO 1: cria uma classe que estende a classe Thread 
class Ola extends Thread {
   String msg;
   
   //--construtor
   public Ola(String m) { 
      msg = m; 
   }

   //--metodo executado pela thread
   public void run() {
      System.out.println(msg); 
   }
}


