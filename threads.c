#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void* thread_func(void* loop){
    for(int i=1; i<=(*(int*)loop); i++){
        printf("%d\n", i);
    }
    pthread_exit(0);
}


int main(int argc, char* argv[]){
    pthread_t t1, t2;
    pthread_t n = 3, m = 4;

    pthread_create(&t1, NULL, &thread_func, (void*)&n);
    pthread_create(&t2, NULL, &thread_func, (void*)&m);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

