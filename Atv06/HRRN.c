#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#define QTD_PROCESS 5

//Estrutura processo que terá os atributos de um processo
typedef struct processo { 
	int id;    //nome do processo
	int at, bt, ct, wt, tt;  //arrival time, burst time, completion time, waiting time e turn around time
	int concluido; //assume 1 para processo concluido ou 0 para processo inconcluido
	float ntt; //normalized turn around time
	int prioridade; //prioridade do processo (varia entre 1 e 5)
}Processo; 

//Função para ordenar a lista de processos baseado em seus arrival time 
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

//Função para gerar numero aleatorio tanto para arrival time quanto para burst time
int gerarNumAleatorio(){
	int num = rand() % 10;
	while(num == 0)   //tratamento para evitar o caso de gerar um numero igual a zero
		num = rand() % 10;
	return num;
}

void main() 
{ 
	int i, j, t; 
	float somaWt = 0, somaTt = 0, somaBt = 0; 
	Processo filaP[QTD_PROCESS]; //vetor para armazenar a fila de processos

	//chamada de srand para evitar que cada execução gere os mesmos numeros
	srand(time(NULL));

	//inicializando cada posição do vetor
	for (i = 0; i < QTD_PROCESS; i++) { 
		filaP[i].id = i+1; 
		filaP[i].at = gerarNumAleatorio(); 
		filaP[i].bt = gerarNumAleatorio(); 
		filaP[i].concluido = 0; 
		somaBt += filaP[i].bt; 
	} 

	ordenar(filaP); //ordenar processos baseado em seus arrival time
	
	for (t = filaP[0].at; t <= somaBt;) { 
		
		float taxa_min = -9999;  //Limite mínimo da taxa de resposta
		float taxa_resposta; 

		int proxP;  //armazena qual o próximo processo a ser chamado
		for (i = 0; i < QTD_PROCESS; i++) { 
			// Check if the process has arrived and is Inconcluido 
			if (filaP[i].at <= t && filaP[i].concluido != 1) { 

				// Calculating the Response Ratio 
				taxa_resposta = (filaP[i].bt + (t - filaP[i].at)) / filaP[i].bt; 

				// Checking for the Highest Response Ratio 
				if (taxa_min < taxa_resposta) { 

					// Storing the Response Ratio 
					taxa_min = taxa_resposta; 

					// Storing the  proxPation 
					proxP = i; 
				} 
			} 
		} 

		//atualizando o tempo decorrido
		t += filaP[proxP].bt;   

		//calculando o tempo de espera (wt) 
		filaP[proxP].wt = t - filaP[proxP].at - filaP[proxP].bt; 

		//somatório do tempo de espera
		somaWt += filaP[proxP].wt; 

		//calculando Turn Around Time (tt) 
		filaP[proxP].tt = t - filaP[proxP].at; 

		//calculando o somatório de todos os turn around time
		somaTt += filaP[proxP].tt; 

		//calculando Normalized Turn Around Time 
		filaP[proxP].ntt = ((float)filaP[proxP].tt / filaP[proxP].bt); 

		//atualizando o status de concluido para 1 (processo concluido) 
		filaP[proxP].concluido = 1;  
	} 
	printf("\nAvg. Waiting Time: %f\n", somaWt / QTD_PROCESS); 
	printf("Avg. Turn Around Time: %f\n", somaTt / QTD_PROCESS); 
	printf("Avg. Processor utilization: %f\n", somaBt / QTD_PROCESS); 
} 