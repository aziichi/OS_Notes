#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int pipefds[2];
    char send_message[13] = "Hello world!";
    char read_message[13];

    int rc = pipe(pipefds);

    if(rc<0){
        printf("Pipe failed.");
        exit(1);
    } 

    int pid = fork();

    if(pid<0){
        printf("Fork failed.");
        exit(1);
    }
    else if(pid==0){
        // Child
        read(pipefds[0], read_message, sizeof(read_message));
        printf("%s\n", read_message);
        exit(0);
    }
    else{
        // Parent 
        write(pipefds[1], send_message, sizeof(send_message));
        wait(NULL);
        exit(0);
    }

    return 0;
}