/*
References: 
https://www.geeksforgeeks.org/wait-system-call-c/
https://www.ti-enxame.com/pt/c/como-obter-pid-filho-em-c/942340092/
*/
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
        printf("[CHILD]: PID %d, starts counting:\n",getpid());
        int i=1;
        while(i<=100){
            printf("[CHILD]: i = %d\n",i);
            i++;
        }
    }else{
        printf("[PARENT]: PID %d, waits for childs with PID %d\n",getpid(),pid);
        wait(NULL);
        printf("[PARENT]: Child with PID %d finished and unloaded\n",pid);
        return 0;
    }
    return 0;
}