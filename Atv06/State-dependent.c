#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define CENARIO "cenario5.txt" //alternar entre "cenario1", "cenario2", "cenario3", "cenario4" e "cenario5" para simular cada um
#define QUANTUM_MIN 1

//Estrutura processo que terá os atributos de um processo
typedef struct processo { 
	int id;    //nome do processo
	float at, rt, wt, tt;  //arrival time, response time, waiting time e turn around time
	float bt;   //burst time
    float salvarEstado; //armazena o quanto do burst time ainda falta para o processo finalizar
    int concluido; //assume 1 para processo concluido ou 0 para processo inconcluido
	int prioridade; //prioridade do processo (4: baixa; 3:média; 2:alta; 1:muito alta)
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
float somaWt=0, somaTt=0, somaBt=0;

//Função para atribuir -1 em todas as posições de um vetor
void resetarVetor(int *vet, int tam){
	for (int j=0; j < tam; j++){
		vet[j] = -1;
	}
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

void inserirNafilaP(Processo **fila, int id, int arriv, int prio, int burst, int bloq)
{
    Processo *aux, *novo = malloc(sizeof(Processo));
    if (novo){
        novo->id = id;
        novo->at = arriv;
        novo->bt = burst;
        novo->salvarEstado = burst;
		somaBt += novo->bt;
        novo->concluido = 0;
        novo->prioridade = prio;
        novo->prox = NULL;
        if (*fila == NULL){
            *fila = novo;
        }else{
            aux = *fila;
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

void imprimirFila(Processo *fila)
{
    if (fila == NULL){
        printf("A fila esta vazia\n");
    }else{
        printf("\n----------Processos---------\n");
        printf("\nID\tTurnAround\tBurst Time\tArrival time\tResponse Time\tWaiting Time\tPrioridade\n"); 
        while(fila){
            printf("%d\t%.2f\t\t%.2f\t\t%.2f\t\t", fila->id, fila->tt, fila->bt, fila->at); 
            printf("%.2f\t\t%.2f\t\t%d\n", fila->rt, fila->wt, fila->prioridade); 
            fila = fila->prox;
        }
    }
}

//Função para verificar se existe algum processo na fila que ainda não foi concluído
//Retorna 1 se existir algum processo não concluído.
//Retorna 0 caso todos os processos estejam concluídos.
int verificarProcessos(Processo *filaP){
    while(filaP){
        if(filaP->concluido == 0)
            return 1;
        filaP = filaP->prox;
    }
    
    return 0;
}

//Verificar quantos processos estão já prontos baseado em seus arrival time
int verificarProntos(Processo *filaP, int tempo){
    int n=0;
    while(filaP){
        if(filaP->at <= tempo)
            n++;
        filaP = filaP->prox;
    }
    
    return n;
}

//Buscar o id de um processo na fila de execução para verificar
//se ele já foi executado alguma vez
int buscarId(Processo2 *filaE, int id){
    while(filaE){
        if(filaE->id == id)
            return 1;
        filaE = filaE->prox;
    }
    
    return 0;
}

//Função para extrair os dados de uma linha do arquivo e armazenar em um nó da fila de processos
void extrairDoArquivo(Processo **filaP, char *linha_arq) 
{ 
	
	int num[6];
	resetarVetor(num, sizeof(num)/sizeof(num[0]));
	char c;
	int id, arriv, prio, burst, bloq;

	int qtd_virgula=0;   //verificar em qual virgula o char 'c' se encontra
	int j=0;
	for(int i=0; i < (int)strlen(linha_arq); i++){
		c = linha_arq[i];

		if(c == ',' || c == '\n'){
			int numero=0;
			for (j=0; j < sizeof(num)/sizeof(num[0]); j++){
				if(num[j] >= 0)
					numero = numero * 10 + num[j];
			}

			if(c == '\n'){
				bloq = numero;
			}else{
				qtd_virgula++;
				if(qtd_virgula == 1) id = numero;
				else if(qtd_virgula == 2) arriv = numero;
				else if(qtd_virgula == 3) prio = numero;
				else if(qtd_virgula == 4) burst = numero;
			}
			resetarVetor(num, sizeof(num)/sizeof(num[0]));
			j=0;

		}else{
			num[j] = c - '0';
			j++;
		}
	}
	inserirNafilaP(filaP, id, arriv, prio, burst, bloq);
} 

int main() 
{ 
    clock_t time_Execution = clock();
    
    FILE *cenario = fopen("./cenarios-entrada/" CENARIO, "r");

	int qtd_process=0;
	char linha_arq[25];

	//Enquanto existir linhas no arquivo o loop é executado
	while(fgets(linha_arq, 25, cenario) != NULL){
		extrairDoArquivo(&filaP, linha_arq);
		qtd_process++;
	}
    fclose(cenario);

    //Ordenando a fila
    ordenarFila(filaP);

    cenario = fopen("./cenarios-saida/Algoritmo-State-Dependent/" CENARIO, "w");
    
    //fprintf(cenario, "\n----------Ordem de execucao---------\n");
    //fprintf(cenario, "\nID\tTurnAround\tBurst Time\tArrival time\tResponse Time\tWaiting Time\tPrioridade\n");

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

            //fprintf(cenario, "%d\t%.2f\t\t%.2f\t\t%.2f\t\t", filaP->id, filaP->tt, filaP->bt, filaP->at); 
            //fprintf(cenario, "%.2f\t\t%.2f\t\t%d\n", filaP->rt, filaP->wt, filaP->prioridade); 


            inserirNaFilaE(&filaE, filaP->id);

            if(i == qtd_process){
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
    //printf("\n");

    //imprimirFilaE(filaE);

    filaP = tail;
    //imprimirFila(filaP);

    time_Execution = clock() - time_Execution; //calculo do tempo de execucao do programa
    fprintf(cenario, "\nAvg. Waiting Time: %.2f\n", somaWt / qtd_process); 
	fprintf(cenario, "Avg. Turn Around Time: %.2f\n", somaTt / qtd_process); 
	fprintf(cenario, "Avg. Service time: %.2f\n", somaBt / qtd_process);
    fprintf(cenario, "Throughput: %.2f\n", t/qtd_process);
    fprintf(cenario, "Tempo de execucao do programa: %.2f\n", (double)time_Execution);

    return 0;
} 
