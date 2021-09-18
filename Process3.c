#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*Neste programa eh impresso o PID do processo, seguido pela
chamada da funcao system, que tem como argumento o programa
Process2. O intuito dessa chamada eh executar um segundo
processo e depois retornar para o processo original.*/
int main(void)
{
    printf("PID do programa original: %d\n", getpid());
    system("./Process2");
    printf("Programa secundario finalizado! Programa original volta a ser executado.\n");
    return 0;
}


