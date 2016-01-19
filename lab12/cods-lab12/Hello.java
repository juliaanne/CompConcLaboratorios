/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 12 */
/* Codigo: "Hello World" usando threads em Java */


//--PASSO 1: criar uma classe que implementa a interface Runnable 
class Hello implements Runnable {
   String msg;
   
   //--construtor
	public Hello (String m) { 
      this.msg = m; 
   }

   //--metodo executado pela thread
   public void run() {
      System.out.println(msg); 
   }
}

