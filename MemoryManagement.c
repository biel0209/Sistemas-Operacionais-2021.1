#include <stdio.h>
#include <stdlib.h>
#define TAM 5
#define TAM_2 10

//Recebe como parametros um ponteiro para um vetor de inteiros e
//seu tamanho a fim de realizar sua impressao.
void imprimirVetor(int *v, int tam)
{
    for(int i=1; i<=tam; i++){
        printf("%d\n",*(v+i));
    }
}

/*
Na funcao principal, inicialmente eh alocado memoria para um
ponteiro que aponta para um vetor de inteiros vet(1...n),
onde n eh o seu tamanho. O vet eh entao preenchido com
inteiros que vao de 1 a n e posteriormente eh realizada a
chamada da funcao imprimirVetor, passando como parametro vet
e seu tamanho. Posteriormente eh chamada a funcao realloc
para realocar o dobro de espaco para vet, que passara a ter
tamanho de 2*n. As novas posicoes de vet serao preenchidas e
sera impresso atraves da chamada de imprimirVetor novamente,
passando agora como parametro o seu novo tamanho. Por fim, eh
utilizada a funcao free para liberar a memoria ocupada por
vet.
*/
int main()
{
    int *vet = malloc(TAM*sizeof(int));
    for(int i=1; i<=TAM; i++){
        *(vet+i) = i;
    }
    printf("VET 1:\n");
    imprimirVetor(vet,TAM);
    vet = realloc(vet, TAM_2*sizeof(int));
    for(int i=TAM+1; i<=TAM_2; i++){
        *(vet+i) = i;
    }
    printf("VET 2:\n");
    imprimirVetor(vet,TAM_2);
    free(vet);
    return 0;
}


