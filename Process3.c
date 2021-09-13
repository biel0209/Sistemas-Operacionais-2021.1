#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    printf("PID do programa original: %d\n", getpid());
    system("./Process2");
    printf("Programa secundario finalizado! Programa original volta a ser executado.\n");
    return 0;
}


