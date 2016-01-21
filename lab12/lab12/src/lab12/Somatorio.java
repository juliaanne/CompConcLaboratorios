package lab12;


public class Somatorio extends Thread
{
	public int id;
	public Integer[] resultado;
	public static Integer[] vetorA = {5, 10, 15, 20, 25, 30};
	public static Integer[] vetorB = {3, 6, 9, 12, 15, 18};
	
	public Somatorio(int id, Integer[] resultado)
	{ 
	      this.id = id;
	      this.resultado = resultado;
	      System.out.println("Criada thread " + id);
	}
	
	public void run()
	{
		for (int i=id; i < vetorA.length; i+=2) {
			resultado[i] = vetorA[i]+vetorB[i];
		}
	}
}
