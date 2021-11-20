#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

long i=0;
long contadorMain=0;
long contador1=0;
long contador2=0;

void *threadCode1()
{
    while(1){
        i++;
        contador1++;
    } 
}

void *threadCode2()
{
    while(1){
        i--;
        contador2++;
    } 

}

void manipularSinal(int sinal)
{
    printf("\n[MAIN]: PID %d, i = %ld\n",getpid(),i);
    printf("[MAIN]: main segment iterations: %ld\n",contadorMain);
    printf("[MAIN]: incrThread iterations: %ld\n",contador1);
    printf("[MAIN]: decrThread iterations: %ld\n",contador2);
    exit(EXIT_SUCCESS);
}

int main()
{
    signal(SIGINT, manipularSinal);

    pthread_t threadId1, threadId2;  //criacao de variaveis para armazenar o id de cada thread
    pthread_attr_t threadAttr1, threadAttr2; //criacao de variaveis para armazenar os atributos da thread 

    //essa funcao inicializa o objeto de atributos da thread com valores padrãos.
    pthread_attr_init(&threadAttr1); 
    pthread_attr_init(&threadAttr2); 

    //essa funcao define qual o escopo de contencao da thread sera utilizada
    pthread_attr_setscope(&threadAttr1, PTHREAD_SCOPE_PROCESS);
    pthread_attr_setscope(&threadAttr2, PTHREAD_SCOPE_PROCESS);
    pthread_attr_setscope(&threadAttr1, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setscope(&threadAttr2, PTHREAD_SCOPE_SYSTEM);

    //criacao das threads
    pthread_create(&threadId1,&threadAttr1,threadCode1,NULL);
    pthread_create(&threadId2,&threadAttr2,threadCode2,NULL);

    while(1){
        i++;
        contadorMain++;
    } 

    return 0;
}