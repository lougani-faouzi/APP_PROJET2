#include <omp.h>
#include <stdio.h>
#include <assert.h>

/*Paralléliser avec OpenMP le calcul de la somme suivante*/
#define max_iter_exo1 1000

void exo1(){
         int somme = 0;
         int rank,size;
         //a pour but de recuperer le nombre de threads qui exécutent notre section parallèle
         size=omp_get_num_threads();
         //on recupere le rang 
         rank=omp_get_thread_num();
         int i;
         
#pragma omp parallel shared(somme,size)
{ // les variables somme,size sont partagées pour tous les thread afin de permettre de calculer les sommes partielles simultanémement         
         
#pragma omp master
  {// seul le thread maitre doit afficher le message (par définition il ya un seul tread maitre) 
            printf("Je suis le maitre !\n");
  }

#pragma omp for private (i) reduction(+:somme) schedule(static) 
 /* 
  -Le compteur de boucle i est automatiquement privé à chaque thread.
  -l'équilibrage de la charge de travail est asurée par :schedule(static).
  -reduction :chaque i calcul un résultat partiel selon les plages d'iteration[0,250][250,500][500,750][750,1000] 
  et synchronisent ensuite pour mettre à jour le résultat final.
   */ 
   for(i=rank*(max_iter_exo1/size);i<(rank*(max_iter_exo1/size))+(max_iter_exo1/size);i++)
   {  
	somme=somme+i;
   }



}
/*Vérification du résultat*/
  fprintf(stderr,"Somme %d attendu %d\n",somme, ((max_iter_exo1-1)*max_iter_exo1)/2);
  assert(somme == ((max_iter_exo1-1)*max_iter_exo1)/2);
}


/*
  Expliquer les valeurs des adresses de chacune des variables suivantes: a,b,c,d. Expliquer pourquoi elle sont différentes ou identiques.
*/
int a;
int b;
#pragma threadprivate(b)
void exo2(){
  static int c;
  int d;
  int e;
  int f; 
#pragma omp parallel private(e) shared(f)
  {
    fprintf(stderr,"\tThread %d a %p\n",omp_get_thread_num(),&a);
    fprintf(stderr,"\tThread %d b %p\n",omp_get_thread_num(),&b);
    fprintf(stderr,"\tThread %d c %p\n",omp_get_thread_num(),&c);
    fprintf(stderr,"\tThread %d d %p\n",omp_get_thread_num(),&d);
    fprintf(stderr,"\tThread %d e %p\n",omp_get_thread_num(),&e);
    fprintf(stderr,"\tThread %d f %p\n",omp_get_thread_num(),&f);
  }
    fprintf(stderr,"Thread %d a %p\n",omp_get_thread_num(),&a);
    fprintf(stderr,"Thread %d b %p\n",omp_get_thread_num(),&b);
    fprintf(stderr,"Thread %d c %p\n",omp_get_thread_num(),&c);
    fprintf(stderr,"Thread %d d %p\n",omp_get_thread_num(),&d);
    fprintf(stderr,"Thread %d e %p\n",omp_get_thread_num(),&e);
    fprintf(stderr,"Thread %d f %p\n",omp_get_thread_num(),&f);
}

int main(int argc, char** argv){
  fprintf(stderr,"OpenMP\n");
  
  exo1();
  //exo2();
  
  return 0; 
}
