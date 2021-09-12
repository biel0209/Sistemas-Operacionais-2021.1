#include <stdio.h>
#include <stdlib.h>
#define TAM 5
#define TAM_2 10

void imprimirVetor(int *v, int tam)
{
    for(int i=1; i<=tam; i++){
        printf("%d\n",*(v+i));
    }
}

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


