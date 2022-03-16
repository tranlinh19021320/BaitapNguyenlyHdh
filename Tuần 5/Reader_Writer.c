#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

int wrt=1;
int mutex=1;

void wait(int *a)
{
	while(a<=0);
	a--;
}
void signal(int *a)
{
	a++;
}

int cnt = 1;
int numreader = 0;

void *writer(void *wno)
{   
    wait(&wrt);
    cnt = cnt*2;
    printf("Writer %d modified cnt to %d\n",(*((int *)wno)),cnt);
    signal(&wrt);

}
void *reader(void *rno)
{   
    // Reader acquire the lock before modifying numreader
    wait(&mutex);
    numreader++;
    if(numreader == 1) {
        wait(&wrt); // If this id the first reader, then it will block the writer
    }
    signal(&mutex);
    // Reading Section
    printf("Reader %d: read cnt as %d\n",*((int *)rno),cnt);

    // Reader acquire the lock before modifying numreader
    wait(&mutex);
    numreader--;
    if(numreader == 0) {
        signal(&wrt); // If this is the last reader, it will wake up the writer.
    }
    signal(&mutex);
}

int main()
{   

    pthread_t read[10],write[5];
    int a[10] = {1,2,3,4,5,6,7,8,9,10}; //Just used for numbering the producer and consumer

    for(int i = 0; i < 10; i++) {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(write[i], NULL);
    }

    return 0;
    
}
