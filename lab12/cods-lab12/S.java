//classe da estrutura de dados (recurso) compartilhado entre as threads
class S {
  //recurso compartilhado
  int r;
  //construtor
  public S() { 
     this.r = 0; 
  }

  //operacao sobre o recurso compartilhado
  public void inc() { 
     this.r++; 
  }
  
  /*public void inc() { 
     synchronized(this) {
        this.r++; 
     }
  }*/
  
  // ou...

  /*public synchronized void inc() { 
     this.r++; 
  }*/

  //operacao sobre o recurso compartilhado
  public int get() { 
      return this.r; 
  }
}

