#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define BufferSize 10 // Size of the buffer

int empty=BufferSize;
int full=0;
int in = 0;
int out = 0;
int mutex=1;
int buffer[BufferSize];
//pthread_mutex_t mutex;

void wait(int *a)
{
	while(a<=0);
	a--;
}
void signal(int *a)
{
	a++;
}
void *producer(void *pno)
{   
    int item;
    while(true) {
        item = rand(); // Produce an random item
        wait(&empty);
        wait(&mutex);
        buffer[in] = item;
        printf("Producer %d: Insert Item %d at %d\n", *((int *)pno),buffer[in],in);
        in = (in+1)%BufferSize;
        signal(&mutex);
        signal(&full);
    }
}
void *consumer(void *cno)
{   
    while(true) {
        wait(&full);
        wait(&mutex);
        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from %d\n",*((int *)cno),item, out);
        out = (out+1)%BufferSize;
        signal(&mutex);
        signal(&empty);
    }
}

int main()
{   

    pthread_t pro[5],con[5];

    int a[5] = {1,2,3,4,5}; //Just used for numbering the producer and consumer

    for(int i = 0; i < 5; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(con[i], NULL);
    }



    return 0;
    
}
