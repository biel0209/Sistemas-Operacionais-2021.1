#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define QTD_PROCESS 5
#define QUANTUM 2.4

//Estrutura processo que terá os atributos de um processo
typedef struct processo { 
	int id;    //nome do processo
	float at, rt, wt, tt;  //arrival time, response time, waiting time e turn around time
	float bt;   //burst time
    float salvarEstado; //armazena o quanto do burst time ainda falta para o processo finalizar
    int concluido; //assume 1 para processo concluido ou 0 para processo inconcluido
	int prioridade; //prioridade do processo (1: baixa; 2:média; 3:alta; 4:muito alta)
    struct processo *prox;
}Processo; 

Processo *filaP = NULL; //estrutura dos processos (filaP)

//Função para gerar numero aleatorio
int gerarNumAleatorio(int valor_max){
	int num = rand() % valor_max;
    while(num == 0)   //tratamento para evitar o caso de gerar um numero igual a zero
        num = rand() % valor_max;
	return num;
}

void inserirNafilaP(Processo **filaPP, int id)
{
    Processo *aux, *novo = malloc(sizeof(Processo));
    if (novo){
        novo->id = id;
        novo->at = gerarNumAleatorio(10);
        novo->bt = gerarNumAleatorio(5);
        novo->concluido = 0;
        novo->prioridade = gerarNumAleatorio(5);
        novo->prox = NULL;
        if (*filaPP == NULL){
            *filaPP = novo;
        }else{
            aux = *filaPP;
            while(aux->prox){
                aux = aux->prox;
            }
            aux->prox = novo;
        }
    }else{
        printf("Memoria indisponivel\n");
    }
}

void ordenarFila(Processo *filaP)       //metodo bubblesort
{
    Processo *pi; //referencia do primeiro da fila
    Processo *pj; //referencia do nó que varrerá a fila
    Processo *pfim = NULL; //referencia do ultimo da fila

    for(pi=filaP; pi->prox != NULL; pi = pi->prox){
        for(pj=filaP; pj->prox != pfim; pj = pj->prox){
            if ( pj->at > pj->prox->at ){
                int auxId = pj->id;
                int auxAt = pj->at;
                int auxBt = pj->bt;
                int auxPrioridade = pj->prioridade;

                pj->id = pj->prox->id;
                pj->at = pj->prox->at;
                pj->bt = pj->prox->bt;
                pj->prioridade = pj->prox->prioridade;

                pj->prox->id = auxId;
                pj->prox->at = auxAt;
                pj->prox->bt = auxBt;
                pj->prox->prioridade = auxPrioridade;
            }
        }
        pfim = pj;
    }
}


Processo* removerDaFila(Processo **filaP)
{
    Processo *remover = NULL;
    remover = *filaP;
    *filaP = remover->prox;
    return remover;
}

void imprimirFila(Processo *filaP)
{
    if (filaP == NULL){
        printf("A fila esta vazia\n");
    }else{
        printf("\n----------Processos---------\n");
        printf("\nID\tTurnAround\tBurst Time\tArrival time\tResponse Time\tWaiting Time\n"); 
        while(filaP){
            printf("%d\t%.2f\t\t%.2f\t\t%.2f\t\t", filaP->id, filaP->tt, filaP->bt, filaP->at); 
            printf("%.2f\t\t%.2f\n", filaP->rt, filaP->wt); 
            filaP = filaP->prox;
        }
    }
}

//Função para verificar se existe algum processo na fila que ainda não foi concluído
//Retorna 1 se existir algum processo não concluído.
//Retorna 0 caso todos os processos estejam concluídos.
int verificarProcessos(Processo *filaP){
    if (filaP == NULL){
        printf("A fila esta vazia\n");
    }else{
        while(filaP){
            if(filaP->concluido == 0)
                return 1;
            filaP = filaP->prox;
        }
    }
    return 0;
}

void main() 
{ 
    //chamada de srand para evitar que cada execução gere os mesmos numeros
	srand(time(NULL));
    
    //Inserindo os processos na fila
    for(int i=0; i<QTD_PROCESS; i++){
        inserirNafilaP(&filaP, i+1);
    }

    //Ordenando a fila
    ordenarFila(filaP);

    Processo *tail = filaP; //referencia do primeiro processo para garantir uma fila circular

    imprimirFila(filaP);

    printf("Ordem de execucao:\n");
    int i=1;
    int j=1;
    float somaWt=0, somaTt=0, somaBt=0;
    float t = filaP->at;
    while(verificarProcessos(tail)){
        if(filaP->concluido != 1){
            printf("%d - ",filaP->id);
            if (filaP->salvarEstado - QUANTUM <= 0){ //verificar se o processo foi concluido
                filaP->concluido = 1;
                tail->concluido = 1;
            }else{
                filaP->salvarEstado -= QUANTUM;
            }
            
            //atualizando o tempo decorrido
            t += QUANTUM;   

            //calculando o tempo de espera (wt) 
            filaP->wt = fabs(t - filaP->at - filaP->bt);

            //calculando o tempo de resposta (rt) 
            filaP->rt = fabs(t - filaP->at - filaP->bt);

            //somatório do tempo de espera
            somaWt += filaP->wt; 

            //calculando Turn Around Time (tt) 
            filaP->tt = fabs(t - filaP->at); 

            //calculando o somatório de todos os turn around time
            somaTt += filaP->tt; 
            
            //calculando o somatório de todos burst times
            somaBt += filaP->bt; 

            if(i == QTD_PROCESS){
                filaP = tail;
                i=0;
            }
            else
                filaP = filaP->prox;
            i++;
            j++;
        }else{
            filaP = filaP->prox;
        }
    }
    printf("\n");

    filaP=tail;
    imprimirFila(filaP);	

    printf("\nAvg. Waiting Time: %.2f\n", somaWt / QTD_PROCESS); 
	printf("Avg. Turn Around Time: %.2f\n", somaTt / QTD_PROCESS); 
	printf("Avg. Processor utilization: %.2f\n", somaBt / QTD_PROCESS);
} 
