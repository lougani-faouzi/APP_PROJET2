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
void exo1(){
  int i; 
  int somme = 0;

  for(i = 0; i < max_iter_exo1; i++){
    somme += i; 
  }
  
  /*Vérification du résultat*/
  fprintf(stderr,"Somme %d attendu %d\n",somme, ((max_iter_exo1-1)*max_iter_exo1)/2);
  /*Vérification du résultat de la somme*/
  assert(somme == ((max_iter_exo1-1)*max_iter_exo1)/2);
  /*Vérification du nombre d'iération réalisées par chaque processus MPI*/
  assert(i == max_iter_exo1/size);
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
