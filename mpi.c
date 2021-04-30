#include <mpi.h>
#include <stdio.h>
#include <assert.h>

int rank;
int size;


/*
  Paralléliser avec MPI le calcul de la somme suivante
  Chaque processus MPI devra traiter max_iter_exo1/size éléments
  Le processus 0 doit obtenir le résultat dans la variable somme
*/
#define max_iter_exo1 1000
#define ROOT_RANK 0
void exo1(){

  int plage_debut,plage_fin;

   
  int i; 
  int somme=0,s=0,seuil;
  
  // recuperer le nombre de processus total
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  // recuperer le rang 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  // si le rang recuperé n'est pas egale au rang du processus 0 (ROOT_RANK)
  if(rank!=ROOT_RANK)  
  {
       // on calcule la plage d'iteration 
	seuil=max_iter_exo1/size;
  	plage_debut=rank*max_iter_exo1/size;
  	plage_fin=(rank*max_iter_exo1/size)+seuil; 
       
        //on calcule la somme pour le processus du rang rank 
        for(i=plage_debut;i<plage_fin;i++)
        s=s+i;
        MPI_Reduce(&s, &somme, 1, MPI_INT, MPI_SUM, ROOT_RANK, MPI_COMM_WORLD);
  
  }else // si le rang recuperé est egale au rang du processus 0 
  {
        
        // on fixe la plage d'iteration pour le processus root (processus 0)
      
        plage_fin=max_iter_exo1/size;
        //on calcule la somme pour le processus du rang 0 
        for(i=0;i<plage_fin;i++)
        s=s+i;
        MPI_Reduce(&s, &somme, 1, MPI_INT, MPI_SUM, ROOT_RANK, MPI_COMM_WORLD);
  
  
  /*Vérification du résultat*/
  fprintf(stderr,"Somme %d attendu %d\n",somme, ((max_iter_exo1-1)*max_iter_exo1)/2);
  /*Vérification du résultat de la somme*/
  assert(somme == ((max_iter_exo1-1)*max_iter_exo1)/2);
  /*Vérification du nombre d'iération réalisées par chaque processus MPI*/
  assert(i == max_iter_exo1/size);
  
  }
}


int main(int argc, char** argv){
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  if(rank == 0)
    fprintf(stderr,"MPI %d\n",size);

  /* Vérification du nombre de processus MPI*/
  assert(max_iter_exo1 % size == 0);

  exo1();
  
  
  MPI_Finalize();
  return 0; 
}
