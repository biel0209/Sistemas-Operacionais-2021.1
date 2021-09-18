#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/*
Neste programa eh utilizada as bibliotecas unistd.h e sys/types.h, 
onde a primeira possibilita o uso das funcoes fork() e
getpid(), e a segunda possibilita o uso do tipo pid_t,
indicado para armazenar id de processos.
Na funcao principal, eh chamada a funcao fork(), que tem o
intuito de gerar um novo processo filho com as mesmas
caracteristicas do processo original. O retorno de fork() eh
o id do novo processo gerado, que sera armazenado na variavel
num_pid. A seguir sera testado o id gerado atraves de
condicoes (if): Se o id gerado for menor que zero, houve um erro na 
duplicacao do processo; Se o id for igual a zero, significa que o 
processo que esta atuando eh o filho e seu pid eh impresso.
Se o id for diferente das condicoes acima, significa que o processo 
que esta atuando eh o pai, ou seja, eh o processo original e seu pid eh 
impresso. Ao fim dessas condicoes, o programa vai para o processo filho 
e testa novamente todas as condicoes (por causa da chamada de fork(), 
todo o processo se repete). So entao, o programa finaliza.
*/
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


