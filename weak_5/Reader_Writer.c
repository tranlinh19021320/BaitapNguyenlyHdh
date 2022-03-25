#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdint.h>

sem_t mutex,writeblock;
int data = 0,rcount = 0;

void *reader(void *arg)
{
  sem_wait(&mutex);
  printf("Data read by the reader%ld is %d\n",(intptr_t)arg,data);
  sleep(1);
  sem_post(&mutex);
}

void *writer(void *arg)
{
  sem_wait(&writeblock);
  data++;
  rcount = rcount + 1;
  if (rcount == 1) sem_wait(&mutex);
  sem_post(&writeblock);
  printf("Data writen by the writer%ld is %d\n",(intptr_t)arg,data);
  sleep(1);
  sem_wait(&writeblock);
  rcount = rcount - 1;
  if(rcount==0) sem_post(&mutex);
  sem_post(&writeblock);
}

int main()
{
  int i,b; 
  pthread_t rtid[5],wtid[5];
  sem_init(&mutex,0,1);
  sem_init(&writeblock,0,1);
  for(i=0;i<=5;i++)
  {
    pthread_create(&rtid[i],NULL,reader,(void *) (intptr_t)i);
  }
    pthread_create(&wtid[0],NULL,writer,(void *) (intptr_t)0);
  for(i=0;i<=5;i++)
  {
    pthread_join(rtid[i],NULL);
  }
  return 0;
}
