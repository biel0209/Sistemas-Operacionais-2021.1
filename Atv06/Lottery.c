#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#define QTD_PROCESS 5
#define MAX_TICKETS (QTD_PROCESS*4) //maximo de tickets q pode ser assumido caso todos processos tenham prioridade igual a 4

//Estrutura processo que terá os atributos de um processo
typedef struct processo { 
	int id;    //nome do processo
	int at, bt, rt, wt, tt;  //arrival time, burst time, response time, waiting time e turn around time
	int concluido; //assume 1 para processo concluido ou 0 para processo inconcluido
	float ntt; //normalized turn around time
	int prioridade; //prioridade do processo (1: baixa; 2:média; 3:alta; 4:muito alta)
    int qtd_tickets; //quantidade de tickets que cada processo possui
    int *tickets; //vetor que armazenará quais tickets o processo tem
}Processo; 

Processo filaP[QTD_PROCESS]; //vetor para armazenar a fila de processos
int tickets_distribuidos = 0;   //quantidade de tickets distribuídos
int *tickets_sorteados; //vetor que armazena todos os tickets já sorteados

//Função para gerar numero aleatorio
int gerarNumAleatorio(int valor_max){
	int num = rand() % valor_max;
    while(num == 0)   //tratamento para evitar o caso de gerar um numero igual a zero
        num = rand() % valor_max;
	return num;
}

//Função para ordenar um vetor
void ordenar(Processo *vet) 
{ 
	Processo aux; 
	for (int i = 0; i < QTD_PROCESS - 1; i++) { 
		for (int j = i + 1; j < QTD_PROCESS; j++) { 
			if (vet[i].at > vet[j].at) { 
				aux = vet[i]; 
				vet[i] = vet[j]; 
				vet[j] = aux; 
			} 
		} 
	} 
} 

//definir os tickets de cada processo
void distribuirTickets(){   
    int j;
    int ultimo_ticket = 1;
    for (int i = 0; i < QTD_PROCESS; i++){ 
        j = 0;
        while(j < filaP[i].qtd_tickets){
            filaP[i].tickets[j] = ultimo_ticket;
            j++;
            ultimo_ticket++;
        }
    }
}

//Função para verificar se um ticket já foi sorteado
int verificarTicket(int ticket){
    for (int i = 0; i < QTD_PROCESS; i++) { 
        if(tickets_sorteados[i] == ticket) 
            return 1;
    }
    return 0;
}

//Função para sortear um ticket
int sortearTicket(int indice){
    int ticket = gerarNumAleatorio(tickets_distribuidos+1);
    while (verificarTicket(ticket) == 1){ //tratamento para o caso de gerar um ticket duplicado
        ticket = gerarNumAleatorio(tickets_distribuidos+1);
    }
    tickets_sorteados[indice] = ticket;
    int prox_Processo = -1;
    for (int i = 0; i < QTD_PROCESS; i++) { 
        for (int j = 0; j < filaP[i].qtd_tickets; j++) { 
            if(ticket == filaP[i].tickets[j])
                prox_Processo = i;
        }
	}

    if(filaP[prox_Processo].concluido == 1){
         tickets_sorteados[indice] = 0;

    }
    
    return prox_Processo;
}

//Função para imprimir o vetor de processos
void imprimirProcessos(){
    for (int i = 0; i < QTD_PROCESS; i++) { 
        printf("ID\tPrioridade\tQtd Tickets\tVetor de tickets\n");
		printf("%d\t%d\t\t%d\t", filaP[i].id, filaP[i].prioridade, filaP[i].qtd_tickets);
        printf("\t[");
        for (int j = 0; j < filaP[i].qtd_tickets; j++) { 
            if(j==0)
                printf("%d",filaP[i].tickets[j]);
            else
                printf(",%d",filaP[i].tickets[j]);
        }
        printf("]\n");
	}
}


void main() 
{ 
	srand(time(NULL)); 
    int t;
	float somaWt = 0, somaTt = 0, somaBt = 0; 
    
    //inicializando cada posição do vetor
	for (int i = 0; i < QTD_PROCESS; i++){ 
		filaP[i].id = i+1; 
		filaP[i].at = gerarNumAleatorio(10); 
		filaP[i].bt = gerarNumAleatorio(10); 
        filaP[i].prioridade = gerarNumAleatorio(5);
        filaP[i].qtd_tickets = filaP[i].prioridade;
        filaP[i].tickets = malloc(filaP[i].qtd_tickets * sizeof(int));
		filaP[i].concluido = 0; 
		somaBt += filaP[i].bt; 
        tickets_distribuidos += filaP[i].qtd_tickets;
	}

    distribuirTickets();

    //ordenar(filaP); //ordenar processos baseado em seus arrival time

    imprimirProcessos();

    printf("Qtd de tickets distribuídos: %d\n", tickets_distribuidos);

    printf("\nID\tTurnAround\tBurst Time\tArrival time\tResponse Time\tWaiting Time"); 

    tickets_sorteados = malloc(QTD_PROCESS * sizeof(int));
    int indice=0; //indice para o vetor tickets_sorteados

    for (t = filaP[0].at; t <= somaBt;){

        int proxP = sortearTicket(indice);

        

        //atualizando o tempo decorrido
        t += filaP[proxP].bt;   

        //calculando o tempo de espera (wt) 
        filaP[proxP].wt = t - filaP[proxP].at - filaP[proxP].bt; 

        //calculando o tempo de resposta (wt) 
        filaP[proxP].rt = t - filaP[proxP].at - filaP[proxP].bt; 

        //somatório do tempo de espera
        somaWt += filaP[proxP].wt; 

        //calculando Turn Around Time (tt) 
        filaP[proxP].tt = t - filaP[proxP].at;

        //calculando o somatório de todos os turn around time
        somaTt += filaP[proxP].tt; 

        //atualizando o status de concluido para 1 (processo concluido) 
        filaP[proxP].concluido = 1;

        indice++;

        printf("\n%d\t%d\t\t%d\t\t%d\t\t", filaP[proxP].id, filaP[proxP].tt, filaP[proxP].bt, filaP[proxP].at); 
        printf("%d\t\t%d\t\t%d", filaP[proxP].rt, filaP[proxP].wt, t); 

        
    }

    printf("\nAvg. Waiting Time: %f\n", somaWt / QTD_PROCESS); 
	printf("Avg. Turn Around Time: %f\n", somaTt / QTD_PROCESS); 
	printf("Avg. Processor utilization: %f\n", somaBt / QTD_PROCESS); 

    for (int i = 0; i < QTD_PROCESS; i++){ 
        printf("%d ",tickets_sorteados[i]);
    }
    printf("\n");
} 