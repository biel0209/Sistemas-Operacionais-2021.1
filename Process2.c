#include <stdio.h>
#include <unistd.h>

//Este programa imprime o PID do seu processo.
int main()
{
    printf("PID do programa executado pelo original: %d\n", getpid());
    return 0;
}


