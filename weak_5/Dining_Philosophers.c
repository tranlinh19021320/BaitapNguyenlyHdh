#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define  N 5                    // number of philosophers
#define  THINKING 0             // philosopher is thinking
#define  HUNGRY 1               // philosopher is trying to get forks
#define  EATING 2               // philosopher is eating

#define LEFT (i+N-1)%N    // number of i's left neighbor
#define RIGHT (i+1)%N     // number of i's right neighbor

int state[N];             // array to keep track of everyone's state
pthread_mutex_t mutex;        // mutual exclusion for critical regions
int s[N]={0, 0, 0, 0, 0}; 
                          // one semaphore per philosopher
sem_t mo;            // for synchronized cout

int myrand(int min, int max) {
  return min+rand() % (max+1-min);
}

void test(int i) {        // i: philosopher number, from 0 to N-1
  if (state[i] == HUNGRY 
   && state[LEFT] != EATING && state[RIGHT] != EATING) {
    state[i] = EATING;
    s[i]=s[i]+1;
  }
}

void think(int i) {
  int duration = myrand(2000, 4000);
  {
    sem_wait(&mo);
    printf("%d thinks %d ms\n",i,duration);
    sem_post(&mo);
  }
  usleep(duration);
}

void take_forks(int i) {  // i: philosopher number, from 0 to N-1
  pthread_mutex_lock(&mutex);         // enter critical region
  state[i] = HUNGRY;      // record fact that philosopher i is hungry
  {
    sem_wait(&mo);
    printf("\t\t%d is hungry\n",i);
    sem_post(&mo);
  }
  test(i);                // try to acquire 2 forks
  pthread_mutex_unlock(&mutex);         // exit critical region
  s[i]=s[i]-1;     // block if forks were not acquired
} 

void eat(int i) {
  int duration = myrand(2000, 3000);
  {
    sem_wait(&mo);
    printf("\t\t\t\t %d eats %d ms\n",i,duration);
    sem_post(&mo);
  }
  usleep(duration);
}

void put_forks(int i) {   // i: philosopher number, from 0 to N-1
  pthread_mutex_lock(&mutex);         // enter critical region
  state[i] = THINKING;    // philosopher has finished eating
  test(LEFT);             // see if left neighbor can now eat
  test(RIGHT);            // see if right neighbor can now eat
  pthread_mutex_unlock(&mutex);      // exit critical region
}

void *philosopher(void *k) {
  int *a=k;
  int i=*a; // i: philosopher number, from 0 to N-1
  while (true) {          // repeat forever
    think(i);             // philosopher is thinking
    take_forks(i);        // acquire two forks or block
    eat(i);               // yum-yum, spaghetti
    put_forks(i);         // put both forks back on table
  }
}

int main() {
  pthread_t phi[5];
  pthread_mutex_init(&mutex,NULL);
  sem_init(&mo,0,1);
  int a[5]={1,2,3,4,5};
  for(int i=0;i<5;i++){
  	pthread_create(&phi[i],NULL,(void*) philosopher,(void*)&a[i]);
  }
  for(int i=0;i<5;i++){
  	pthread_join(phi[i],NULL);
  }
  pthread_mutex_destroy(&mutex);
  sem_destroy(&mo);
}

