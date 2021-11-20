#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int i = 0;  //variavel contador global

/*A funcao manipularSinal eh acionada com o sinal CONTROL + C e 
chamada a funcao kill para matar o processo atraves do sinal SIGKILL, 
imprimindo na tela o pid e contador do processo finalizado.*/
void manipularSinal(int sinal)
{
    printf("\nProcess %d killed, i = %d\n",getpid(),i);
    kill(getpid(), SIGKILL);
}


int main()
{
    pid_t pid;

    signal(SIGINT, manipularSinal);
 
    pid = fork();
    if (pid < 0){
        printf("Erro! Fork falhou!\n");
    }else if(pid == 0){
        printf("[CHILD]: PID %d, PPID %d starts counting.\n",getpid(),getppid());
        while(1)
            i++;
    }else{
        printf("[PARENT]: PID %d, starts counting.\n",getpid());
        while(1)
            i++;
    }
    return 0;
}