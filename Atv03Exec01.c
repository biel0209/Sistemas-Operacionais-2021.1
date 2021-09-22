/*
References: 
https://man7.org/linux/man-pages/man2/getpid.2.html
https://linuxhint.com/signal_handlers_c_programming_language/
https://www.gnu.org/software/libc/manual/html_node/Termination-Signals.html

*/
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/*
A funcao manipularSinal vai receber um parametro do tipo int como argumento
e ira verificar se ele eh um SIGINT ou SIGTERM e exibir uma mensagem de acordo
com cada caso.
*/
void manipularSinal(int sinal)
{
    if(sinal == SIGINT)
        printf("\nFuncao principal interrompida por SIGINT(CTRL+C)!\n");
    else if(sinal == SIGTERM)
        printf("\nFuncao principal interrompida por SIGTERM(Comando kill)!\n");
}


int main()
{
    pid_t pidAtual, meuPid, pidPai;

    //Os sinais SIGINT e SIGTERM sao passados para a funcao manipularSinal
    signal(SIGINT, manipularSinal);
    signal(SIGTERM, manipularSinal);

    pidAtual = fork();
    if (pidAtual < 0){
        printf("Erro! Fork falhou!\n");
    }else if(pidAtual == 0){
        meuPid = getpid();
        pidPai = getppid();
        do{
            printf("[CHILD]: PID %d, PPID %d\n",meuPid,pidPai);
        }while(1);
        return 0;
    }else{
        meuPid = getpid();
        pidPai = getppid();
        do{
            printf("[PARENT]: PID %d, PPID %d\n",meuPid,pidPai);
        }while(1);
        return 0;
    }
    return 0;
}