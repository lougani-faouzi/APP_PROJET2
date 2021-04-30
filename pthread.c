#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/*
Convertir le code du fichier openmp.c exo1 en code utilisant uniquement les fonctions pthread
*/

#define NB_THREADS_EXO1 4
#define max_iter_exo1 1000

typedef struct 
{       int somme;     // corresspond a variable qui contiendra la somme 
	int val_iter;  // correspond ala valeur de l'iteration i dans le code openmp 
	pthread_mutex_t mut;// un verrou mut
}infos;


void *critique(void *a)
{
   infos *f=NULL;
   int i;
   
   //on récupére l'identifiant du thread crée par pthread_create (notre cas la fonction exo1())
   pthread_t thread_id = pthread_self();

   //verouiller le mutex mut apres avoir fait un cast de *a vers le type infos  
   f=(infos*)a;
   pthread_mutex_lock(&(*f).mut);
   
   // calculer de la somme  
   for(i=(*f).val_iter;i<max_iter_exo1;i++)
   {   
	(*f).somme=(*f).somme+(*f).val_iter;
	(*f).val_iter=(*f).val_iter+1;
   }

   // libérer le mutex mut
   pthread_mutex_unlock(&(*f).mut);
   pthread_exit(NULL);
   
}


void  exo1(){


    int i=0,j=0; 
    infos f;
    
    // allocation du tableau qui va contenir les threads
    pthread_t *threads=NULL;
    threads = malloc(NB_THREADS_EXO1*sizeof(pthread_t));
    
    // Initialisation du type 
    f.val_iter = 0;
    f.somme = 0;
    pthread_mutex_init(&f.mut,NULL);

    //création des threads
     while(i<NB_THREADS_EXO1)
     {
        //chaque thread execute la fonction critique() en prenant comme argument la structure f
	pthread_create(&*(threads+i),NULL,critique,&f);
	i++;
     }
    
     //fin d'execution des threads
     while(i<NB_THREADS_EXO1)
     {
       pthread_join(*(threads+i),NULL);
       j++;
     }
    
      // Destruction du mutex
      pthread_mutex_destroy(&f.mut);
   
      //desalocation du tableau threads
      free(threads);


 /*Vérification du résultat*/
  fprintf(stderr,"Somme %d attendu %d\n",f.somme, ((max_iter_exo1-1)*max_iter_exo1)/2);
  assert(f.somme == ((max_iter_exo1-1)*max_iter_exo1)/2);
}

/*
  expliquer les valeur des adresses de chacune des variables suivantes: a,b,c,d. Expliquer pourquoi elle sont différentes ou identiques
*/
#define NB_THREADS 4
int a;
__thread int b;

void* func(void* arg){
  static int c;
  int d;
  static __thread int e;
 
 fprintf(stderr,"\tThread %p a %p\n",(void*)pthread_self(),&a);
    fprintf(stderr,"\tThread %p b %p\n",(void*)pthread_self(),&b);
    fprintf(stderr,"\tThread %p c %p\n",(void*)pthread_self(),&c);
    fprintf(stderr,"\tThread %p d %p\n",(void*)pthread_self(),&d);
    fprintf(stderr,"\tThread %p e %p\n",(void*)pthread_self(),&e);
}

void exo2(){
    pthread_t tid[NB_THREADS];
    int i;

    for(i = 0; i < NB_THREADS; i++){
      pthread_create(&(tid[i]),NULL,func,NULL);
    }
    
    for(i = 0; i < NB_THREADS; i++){
      pthread_join(tid[i], NULL);
    }
  
    fprintf(stderr,"Thread %p a %p\n",(void*)pthread_self(),&a);
    fprintf(stderr,"Thread %p b %p\n",(void*)pthread_self(),&b);
    
}
int main(int argc, char** argv){
  fprintf(stderr,"PThread\n");
  
  exo1();
  exo2();
  
  return 0; 
}
