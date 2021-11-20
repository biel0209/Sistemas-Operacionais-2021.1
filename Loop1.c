#include <stdio.h>
#include <stdlib.h>

/*Este programa realiza um calculo, armazena o resultado novetor vet e o imprime. Esse processo ocorre 10.000 vezes.*/
int main()
{       
    int i=0;
    double vet[10000];
    while (i<10000){
        vet[i] = i*i*i/i*i*i;
        printf("%f\n",vet[i]);
        i++;
    }
    return 0;
}
