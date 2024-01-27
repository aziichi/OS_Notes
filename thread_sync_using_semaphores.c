#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

long long count = 0;

sem_t mutex;

void* thread_func(void* arg){
    //using binary semaphore

    sem_wait(&mutex);
    for(int i=0; i<(*(int*)arg); i++){
        count++;
    }
    sem_post(&mutex);
    pthread_exit(0);
}


int main(int argc, char* argv[]){
    pthread_t t1, t2, n=100000;
    sem_init(&mutex, 0, 1);
    
    pthread_create(&t1, NULL, &thread_func, (void*)&n);
    pthread_create(&t2, NULL, &thread_func, (void*)&n);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("%lld\n", count);

    return 0;
}