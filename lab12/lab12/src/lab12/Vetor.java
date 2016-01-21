package lab12;

import java.util.ArrayList;

public class Vetor {
	static ArrayList<Integer> vetorA = new ArrayList<>();
	
	public Vetor()
	{
		vetorA.add(10); 
		vetorA.add(25);
		vetorA.add(20);
		vetorA.add(5);
		vetorA.add(15);
		vetorA.add(0);
		vetorA.add(30);
		
		//vetorB.add(39);
		//vetorB.add(5);
		//vetorB.add(61);
		//vetorB.add(44);
		//vetorB.add(12);
		//vetorB.add(21);
		//vetorB.add(34);
	}
	
	public int get(int pos)
	{
		int valor = vetorA.get(pos);
		return valor;
	}
}
