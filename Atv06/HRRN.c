#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define CENARIO "cenario1.txt"

//Estrutura processo que terá os atributos de um processo
typedef struct processo{ 
	int id;    //nome do processo
	int at, bt, rt, wt, tt;  //arrival time, burst time, response time, waiting time e turn around time
	int concluido; //assume 1 para processo concluido ou 0 para processo inconcluido
	float ntt; //normalized turn around time
	int blTime; //tempo de bloqueio
	int prioridade; //prioridade do processo (varia entre 1 e 5)
	struct processo *prox;
}Processo; 

int somaWt = 0, somaTt = 0, somaBt = 0; 

//Função para atribuir -1 em todas as posições de um vetor
void resetarVetor(int *vet, int tam){
	for (int j=0; j < tam; j++){
		vet[j] = -1;
	}
}

void inserirNafilaP(Processo **fila, int id, int arriv, int prio, int burst, int bloq)
{
    Processo *aux, *novo = malloc(sizeof(Processo));
    if (novo){
        novo->id = id;
        novo->at = arriv;
        novo->bt = burst;
		somaBt += novo->bt;
        novo->concluido = 0;
        novo->prioridade = prio;
		novo->blTime = bloq;
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

void ordenarFila(Processo *fila)       //metodo bubblesort
{
    Processo *pi; //referencia do primeiro da fila
    Processo *pj; //referencia do nó que varrerá a fila
    Processo *pfim = NULL; //referencia do ultimo da fila

    for(pi=fila; pi->prox != NULL; pi = pi->prox){
        for(pj=fila; pj->prox != pfim; pj = pj->prox){
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


Processo* removerDaFila(Processo **fila)
{
    Processo *remover = NULL;
    remover = *fila;
    *fila = remover->prox;
    return remover;
}

void imprimirFila(Processo *fila)
{
    if (fila == NULL){
        printf("A fila esta vazia\n");
    }else{
        printf("\n----------Processos---------\n");
        printf("\nID\tTurnAround\tBurst Time\tArrival time\tResponse Time\tWaiting Time\tPrioridade\tTime Blocked\n"); 
        while(fila){
            printf("%d\t%d\t\t%d\t\t%d\t\t", fila->id, fila->tt, fila->bt, fila->at); 
            printf("%d\t\t%d\t\t%d\t\t%d\n", fila->rt, fila->wt, fila->prioridade, fila->blTime); 
            fila = fila->prox;
        }
    }
}

int* extrairDoArquivo(Processo **filaP, char *linha_arq) 
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

void main() 
{ 
	int i, j, t; 
	Processo *filaP = NULL; //fila de processos prontos
	Processo *bloqueados = NULL; //fila de processos bloqueados
	FILE *cenario = fopen("./cenarios-entrada/" CENARIO, "r");

	int qtd_process=0;
	char linha_arq[25];
	//Enquanto existir linhas no arquivo o loop é executado
	while(fgets(linha_arq, 25, cenario) != NULL){
		extrairDoArquivo(&filaP, linha_arq);
		qtd_process++;
	}

	fclose(cenario);

	ordenarFila(filaP); //ordenar processos baseado em seus arrival time

	//imprimirFila(filaP);
	
	Processo *tail = filaP;  //referencia do primeiro nó da fila

	cenario = fopen("./cenarios-saida/" CENARIO, "w");
	
	fprintf(cenario, "\n----------Ordem de execucao---------\n");
    fprintf(cenario, "\nID\tTurnAround\tBurst Time\tArrival time\tResponse Time\tWaiting Time\tPrioridade\n"); 
	for (t = filaP->at; t <= somaBt;) { 
		
		float taxa_min = -9999;  //Limite mínimo da taxa de resposta
		float taxa_resposta; 

		Processo *aux;  //realizar iteração
		Processo *proxP;  //armazena qual o próximo processo a ser chamado
		for (aux = filaP; aux; aux = aux->prox) { 
			// Check if the process has arrived and is Inconcluido 
			if (aux->at <= t && aux->concluido != 1) { 

				// Calculating the Response Ratio 
				taxa_resposta = (aux->bt + (t - aux->at)) / aux->bt; 

				// Checking for the Highest Response Ratio 
				if (taxa_min < taxa_resposta) { 

					// Storing the Response Ratio 
					taxa_min = taxa_resposta; 

					// Storing the  proxPation 
					filaP = aux; 
				} 
			} 
			
		} 

		

		//atualizando o tempo decorrido
		t += filaP->bt;   

		//calculando o tempo de espera (wt) 
		filaP->wt = t - filaP->at - filaP->bt; 

		//calculando o tempo de resposta (rt) 
		filaP->rt = t - filaP->at - filaP->bt; 

		//somatório do tempo de espera
		somaWt += filaP->wt; 

		//calculando Turn Around Time (tt) 
		filaP->tt = t - filaP->at; 

		//calculando o somatório de todos os turn around time
		somaTt += filaP->tt; 

		//calculando Normalized Turn Around Time 
		filaP->ntt = ((float)filaP->tt / filaP->bt); 

		//atualizando o status de concluido para 1 (processo concluido) 
		filaP->concluido = 1;  

		fprintf(cenario, "%d\t%d\t\t%d\t\t%d\t\t", filaP->id, filaP->tt, filaP->bt, filaP->at); 
        fprintf(cenario, "%d\t\t%d\t\t%d\n", filaP->rt, filaP->wt, filaP->prioridade); 

		filaP = tail;

		
	} 

	fprintf(cenario, "\nAvg. Waiting Time: %d\n", somaWt / qtd_process); 
	fprintf(cenario, "Avg. Turn Around Time: %d\n", somaTt / qtd_process); 
	fprintf(cenario, "Avg. Service time: %d\n", somaBt / qtd_process); 
	
} 