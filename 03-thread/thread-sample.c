#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<time.h>
#include<sys/time.h>

#define SIZE (int)1e8
#define RANDOM_MAX (int) 0x7fffffff
int array[SIZE];

// Structure contenant les résultats des tests
typedef struct sTestResults {
    int min;
    int max;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
} TestResults;

static TestResults results = {
    .mutex = PTHREAD_MUTEX_INITIALIZER
  };

// Structure contenant les indexes des sous-tableaux découpés
typedef struct sIndexArray{
    int start;
    int end;
} IndexArray;

struct timeval startTime, endTime;

void initArray(){
  int i;
  for(i=0; i<SIZE; i++){
    array[i] = rand() % RANDOM_MAX;
  }
}

void printMin(){
  int i;
  for(i=0; i<SIZE; i++){
    if(array[i] < results.min ){
      results.min  = array[i];
    }
  }
  printf("Valeur minimum du tableau : %d\n", results.min);
}

void printMax(){
  int i;
  for(i=0; i<SIZE; i++){
    if(array[i] > results.max ){
      results.max  = array[i];
    }
  }
  printf("Valeur maximum du tableau : %d\n", results.max);
}

// Trouve la valeur minimum du tableau global
void * findMinArray(void* args){
  IndexArray * indexArray = (IndexArray *) args;

  pthread_mutex_lock(&results.mutex);

  int i = indexArray->start;
  while(i<indexArray->end){
    if(array[i] < results.min ){
      results.min  = array[i];
    }
    i++;
  }

  //printf("min : %d\n", results.min);
  pthread_mutex_unlock(&results.mutex);
  
}

// Trouve la valeur maximum du tableau global
void * findMaxArray(void* args){
  IndexArray * indexArray = (IndexArray *) args;

  pthread_mutex_lock(&results.mutex);

  int i = indexArray->start;
  while(i<indexArray->end){
    if(array[i] > results.max ){
      results.max  = array[i];
    }
    i++;
  }

  //printf("max : %d\n", results.min);
  pthread_mutex_unlock(&results.mutex);
}

// Découpe le tableau global en sous-tableaux en fonction du nombre de threads
IndexArray * splitArray(int nbThreads){
  
  int sizeOfSplittedArray = SIZE / nbThreads;
  int restOfSubArray = SIZE % nbThreads;
  IndexArray * indexArray = malloc(sizeof(IndexArray) * nbThreads);

  int i = 0;
  while(i<nbThreads){
    indexArray[i].start = i * sizeOfSplittedArray;
    indexArray[i].end = indexArray[i].start + sizeOfSplittedArray; 

    if(restOfSubArray > 0 && i == nbThreads-1){
      indexArray[i].end+=restOfSubArray;
    }
    i++;
  }
  return indexArray;
}

// Créer le nombre de threads spécifié(s) en paramètre
void createThreads(int nbThreads, void * (findMinOrMaxTab)(void *)){
  
  pthread_t threads[nbThreads];
  IndexArray * indexArray = splitArray(nbThreads);

  gettimeofday(&startTime, NULL);

  int i = 0;
  while(i<nbThreads){
    pthread_create(&threads[i], NULL, findMinOrMaxTab, &indexArray[i]);
    i++;
  }

  i = 0;
  while(i<nbThreads){
    pthread_join(threads[i], NULL);
    i++;
  }

  gettimeofday(&endTime, NULL);
  printf("Temps de recherche avec %d threads : %ld ms\n", nbThreads, (endTime.tv_sec - startTime.tv_sec) * 1000 + (endTime.tv_usec - startTime.tv_usec) / 1000);
}

// Lancer les threads pour trouver les valeurs minimum et maximum du tableau global
void startThreads(int nbThreads, void * (findMinOrMaxTab)(void *)){
  
  results.min = RANDOM_MAX;
  results.max = 0;

  if(nbThreads > SIZE){
    printf("Le nombre de threads est supérieur à la taille du tableau global");
  }
  else if (nbThreads > 0){
    createThreads(nbThreads, findMinOrMaxTab);
  }
  // on recherche sans threads
  else {
    IndexArray indexArray = {0, SIZE};

    gettimeofday(&startTime, NULL);
    findMinArray((void *) &indexArray);
    gettimeofday(&endTime, NULL);
    printf("Temps de recherche du MIN avec %d threads : %ld ms\n", nbThreads, (endTime.tv_sec - startTime.tv_sec) * 1000 + (endTime.tv_usec - startTime.tv_usec) / 1000);

    gettimeofday (&startTime, NULL);
    findMaxArray((void *) &indexArray);
    gettimeofday (&endTime, NULL);
    printf("Temps de recherche du MAX avec %d threads : %ld ms\n", nbThreads, (endTime.tv_sec - startTime.tv_sec) * 1000 + (endTime.tv_usec - startTime.tv_usec) / 1000);

  }
}

int main(int argc, char const *argv[])
{
  initArray();

  printf("Recherche du MIN et du MAX avec 0 thread\n");
  startThreads(0, NULL);

  printf("\n\n");

  printf("Recherche du MIN et du MAX avec 2 threads\n");
  startThreads(2, findMinArray);
  startThreads(2, findMaxArray);

  printf("\n\n");

  printf("Recherche du MIN et du MAX avec 4 threads\n");
  startThreads(4, findMinArray);
  startThreads(4, findMaxArray);

  printf("\n\n");

  printf("Recherche du MIN et du MAX avec 8 threads\n");
  startThreads(8, findMinArray);
  startThreads(8, findMaxArray);


  printf("\n\n");

  printMin();
  printMax();

  return 0;
}
