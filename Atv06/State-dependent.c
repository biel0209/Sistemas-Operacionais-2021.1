#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define QTD_PROCESS 5
#define QUANTUM_MIN 0.2

//Estrutura processo que terá os atributos de um processo
typedef struct processo { 
	int id;    //nome do processo
	float at, rt, wt, tt;  //arrival time, response time, waiting time e turn around time
	float bt;   //burst time
    float salvarEstado; //armazena o quanto do burst time ainda falta para o processo finalizar
    int concluido; //assume 1 para processo concluido ou 0 para processo inconcluido
	int prioridade; //prioridade do processo (1: baixa; 2:média; 3:alta; 4:muito alta)
    int chamada1; //assume 1 quando for a primeira chamada do processo ou 0 se ainda não foi chamado
    struct processo *prox;
}Processo; 

//Estrutura para uma lista de ordem de execução dos processos
typedef struct processo2 { 
	int id;    
    struct processo2 *prox;
}Processo2; 


Processo *filaP = NULL; //instância da estrutura processo que representa a lista de processos
Processo2 *filaE = NULL; //instância da estrutura processo2 que representa a lista de execução dos processos
float quantum = 2.4;

//Função para gerar numero aleatorio
int gerarNumAleatorio(int valor_max){
	int num = rand() % valor_max;
    while(num == 0)   //tratamento para evitar o caso de gerar um numero igual a zero
        num = rand() % valor_max;
	return num;
}

void inserirNaFilaE(Processo2 **filaE, int id)
{
    Processo2 *aux, *novo = malloc(sizeof(Processo2));
    if (novo){
        novo->id = id;
        novo->prox = NULL;
        if (*filaE == NULL){
            *filaE = novo;
        }else{
            aux = *filaE;
            while(aux->prox){
                aux = aux->prox;
            }
            aux->prox = novo;
        }
    }else{
        printf("Memoria indisponivel\n");
    }
}

void imprimirFilaE(Processo2 *filaE)
{
    if (filaE == NULL){
        printf("A fila esta vazia\n");
    }else{
        printf("\n----------Fila de execucao---------\n");
        while(filaE){
            printf("%d - ", filaE->id); 
            filaE = filaE->prox;
        }
    }
}

void inserirNafilaP(Processo **filaP, int id)
{
    Processo *aux, *novo = malloc(sizeof(Processo));
    if (novo){
        novo->id = id;
        novo->at = gerarNumAleatorio(10);
        int temp = gerarNumAleatorio(5);
        novo->bt = temp;
        novo->salvarEstado = temp;
        novo->concluido = 0;
        novo->prioridade = gerarNumAleatorio(5);
        novo->prox = NULL;
        if (*filaP == NULL){
            *filaP = novo;
        }else{
            aux = *filaP;
            while(aux->prox){
                aux = aux->prox;
            }
            aux->prox = novo;
        }
    }else{
        printf("Memoria indisponivel\n");
    }
}

void inserirNafilaP2(Processo **filaP, int id, int at, int bt, 
            int salvarEstado, int concluido, int prioridade)
{
    Processo *aux, *novo = malloc(sizeof(Processo));
    if (novo){
        novo->id = id;
        novo->at = at;
        novo->bt = bt;
        novo->salvarEstado = salvarEstado;
        novo->concluido = concluido;
        novo->prioridade = prioridade;
        novo->prox = NULL;
        if (*filaP == NULL){
            *filaP = novo;
        }else{
            aux = *filaP;
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

//Verificar quantos processos estão já prontos baseado em seus  
int verificarProntos(Processo *filaP, int tempo){
    int n=0;
    if (filaP == NULL){
        printf("A fila esta vazia\n");
    }else{
        while(filaP){
            if(filaP->at <= tempo)
                n++;
            filaP = filaP->prox;
        }
    }
    return n;
}

//Buscar o id de um processo na fila de execução para verificar
//se ele já foi executado alguma vez
int buscarId(Processo2 *filaE, int id){
    if (filaE == NULL){
        printf("A fila esta vazia\n");
    }else{
        
        while(filaE){
            if(filaE->id == id)
                return 1;
            filaE = filaE->prox;
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

    imprimirFila(filaP);


    Processo *tail = filaP; //referencia do primeiro processo para garantir uma fila circular
    Processo *concluidos;

    int i=1;
    int j=1;
    float t = filaP->at;
    float somaWt=0, somaTt=0, somaBt=0;
    while(verificarProcessos(tail) && filaP!=NULL){
        if(filaP->concluido != 1){
            
            if (filaP->salvarEstado - quantum <= 0){ //verificar se o processo foi concluido
                filaP->concluido = 1;
                tail->concluido = 1;
                
            }else{
                filaP->salvarEstado -= quantum;
            }

            //atualizando o tempo decorrido
            t += quantum;   

            //calculando o tempo de espera (wt) 
            filaP->wt = fabs(t - filaP->at - filaP->bt); 

            //calculando o tempo de resposta (rt) 
            if( buscarId(filaE, filaP->id) != 1 )
                filaP->rt = fabs(t - filaP->at - filaP->bt);

            //somatório do tempo de espera
            somaWt += filaP->wt; 

            //calculando Turn Around Time (tt) 
            filaP->tt = fabs(t - filaP->at); 

            //calculando o somatório de todos os turn around time
            somaTt += filaP->tt; 

            //calculando o somatório de todos burst times
            somaBt += filaP->bt; 

            inserirNaFilaE(&filaE, filaP->id);

            if(i == QTD_PROCESS){
                filaP = tail;
                i=0;
            }else
                filaP = filaP->prox;
            i++;
            j++;

            
        }else{
            filaP = filaP->prox;
        }
        if ( (quantum / verificarProntos(tail, t)) >= QUANTUM_MIN ){
            quantum = quantum / verificarProntos(tail, t);
        }
        
    }
    printf("\n");

    imprimirFilaE(filaE);

    filaP = tail;
    imprimirFila(filaP);

    printf("\nAvg. Waiting Time: %.2f\n", somaWt / QTD_PROCESS); 
	printf("Avg. Turn Around Time: %.2f\n", somaTt / QTD_PROCESS); 
	printf("Avg. Processor utilization: %.2f\n", somaBt / QTD_PROCESS);
} 
