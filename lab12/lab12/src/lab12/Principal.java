package lab12;

public class Principal 
{
	static final int nthreads = 4;
	public static Integer[] resultado = new Integer[6];
	
    public static void main (String[] args) 
    {
	   System.out.println("O programa iniciou");
	   
       // Reservando espaço para vetor de threads
       Thread[] threads = new Thread[nthreads];
    
       // Criando threads 
       for (int i=0; i<threads.length; i++)
       {
          threads[i] = new Somatorio(i, resultado);
       } 

       // Iniciando threads
       for (int i=0; i<threads.length; i++)
       {
    	   	threads[i].start();
       }

       // Esperando threads
       for (int i=0; i<threads.length; i++)
       {
    	   	try { threads[i].join(); } catch (InterruptedException e) { return; }
       }

       for (int i = 0; i < resultado.length; i++) {
           System.out.println("Resultado["+i+"] é "+resultado[i]);	
       }
       
       System.out.println("O programa terminou"); 
	}
    
}
