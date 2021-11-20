#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid, pidChild;
    pid = fork();
    if (pid < 0){
        printf("Erro! Fork falhou!\n");
    }else if(pid == 0){
        execlp("./filho", "filho", NULL);
    }else{
        printf("[PARENT]: PID %d, waits for childs with PID %d\n",getpid(),pid);
        wait(NULL);
        printf("[PARENT]: Child with PID %d finished and unloaded\n",pid);
        return 0;
    }
    return 0;
}