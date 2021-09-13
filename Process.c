#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t num_pid = fork();

    if(num_pid < 0)
        printf("Erro!");
    else if (num_pid == 0)
        printf("Este processo é filho! PID: %d\n", getpid());
    else
        printf("Este processo é pai! PID PAI: %d\n", getpid());
    return 0;
}


