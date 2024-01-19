#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int arc, char* argv[]){
    printf("hello (pid: %d)\n", (int) getpid());
    pid_t rc = fork();

    char* args[3];

    if(rc<0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(rc==0){
        // Child
        printf("child (pid: %d)\n", (pid_t) getpid());
        args[0] = "ls";
        args[1] = ".";
        args[2] = NULL;
        execvp(args[0], args);
    }
    else{
        // Parent
        pid_t rc_wait = wait(NULL);
        printf("parent of %d (pid: %d)\n", rc, (pid_t) getpid());
    }

    return 0;
}

