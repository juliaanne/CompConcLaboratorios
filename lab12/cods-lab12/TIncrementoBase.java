/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 12 */
/* Codigo: Acessando variável compartilhada em um programa multithreading Java */
/* -------------------------------------------------------------------*/

//classe da aplicacao
class TIncrementoBase {
   static final int N = 2;

   public static void main (String[] args) {
      //reserva espaço para um vetor de threads
      Thread[] threads = new Thread[N];

      //cria uma instancia do recurso compartilhado entre as threads
      S s = new S();

      //cria as threads da aplicacao
      for (int i=0; i<threads.length; i++) {
         threads[i] = new T(i, s);
      }

      //inicia as threads
      for (int i=0; i<threads.length; i++) {
         threads[i].start();
      }

      //espera pelo termino de todas as threads
      for (int i=0; i<threads.length; i++) {
         try { threads[i].join(); } catch (InterruptedException e) { return; }
      }

      System.out.println("Valor de s = " + s.get()); 
   }
}