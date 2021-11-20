#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#define QTD_PROCESS 5
#define NUM_TICKETS (QTD_PROCESS*4)

//Estrutura processo que terá os atributos de um processo
typedef struct processo { 
	int id;    //nome do processo
	int at, bt, ct, wt, tt;  //arrival time, burst time, completion time, waiting time e turn around time
	int concluido; //assume 1 para processo concluido ou 0 para processo inconcluido
	float ntt; //normalized turn around time
	int prioridade; //prioridade do processo (1 para alta, 2 para média e 3 para baixa)
    int qtd_tickets; 
    int *tickets; //vetor que armazenará quais tickets o processo tem
}Processo; 

Processo filaP[QTD_PROCESS]; //vetor para armazenar a fila de processos


//Função para gerar numero aleatorio
int gerarNumAleatorio(int excluirZero, int valor_max){
	int num = rand() % valor_max;
    if (excluirZero == 1){   //variavel que aponta se o numero gerado pode ou nao ser um zero
        while(num == 0)   //tratamento para evitar o caso de gerar um numero igual a zero
            num = rand() % valor_max;
    }
	return num;
}

//Função para gerar quantidades de tickets aleatória para cada processo
void definirQtdTickets(){
    for (int i = 0; i < QTD_PROCESS; i++){ 
        int add_qtd = gerarNumAleatorio(0, 4); //gerar valor aleatorio 
        int indice_aleat = gerarNumAleatorio(0,QTD_PROCESS); //gerar indice aleatorio
        while(indice_aleat == i) //tratamento para o indice gerado não ser igual ao indice "i" do for
            indice_aleat = gerarNumAleatorio(0,QTD_PROCESS);
        if (filaP[indice_aleat].qtd_tickets > add_qtd){ //tratamento para nao zerar qtd de tickets
            filaP[i].qtd_tickets += add_qtd;  //adicionado tickets na filaP[i] 
            filaP[indice_aleat].qtd_tickets -= add_qtd; //removido tickets na filaP[indice_aleatorio]
        }
        filaP[i].tickets = malloc(filaP[i].qtd_tickets * sizeof(int));
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

void imprimirProcessos(){
    for (int i = 0; i < QTD_PROCESS; i++) { 
        printf("ID\tQtd Tickets\tVetor de tickets\n");
		printf("%d\t%d\t", filaP[i].id, filaP[i].qtd_tickets);
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
	float somaWt = 0, somaTt = 0, somaBt = 0; 
    
    //inicializando cada posição do vetor
	for (int i = 0; i < QTD_PROCESS; i++){ 
		filaP[i].id = i+1; 
		filaP[i].at = gerarNumAleatorio(1,10); 
		filaP[i].bt = gerarNumAleatorio(1,10); 
        filaP[i].qtd_tickets = NUM_TICKETS / QTD_PROCESS;
		filaP[i].concluido = 0; 
        filaP[i].prioridade = gerarNumAleatorio(1,4);
		somaBt += filaP[i].bt; 
	}

    definirQtdTickets();

    distribuirTickets();

    imprimirProcessos();

    sortearTicket();
    

} 