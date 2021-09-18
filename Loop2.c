#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*A funcao buscar realiza uma busca do caracter c no arquivo
IoBound.txt e imprime sua quantidade de ocorrencias.*/
void buscar(char c)
{
    FILE *arq;
    arq = fopen("IoBound.txt","r");   
    int cont = 0;
    char x;
    do{
      x = fgetc(arq);
      if (x == c)
        cont++;       
    }while (x != EOF);
    printf("\nOcorrencias de '%c' no arquivo: %d\n" , c, cont);        
}

/*A funcao principal escaneia uma entrada dada pelo usuario e
envia para funcao buscar.*/
int main()
{
    char c;
    printf("Digite um caracter a ser pesquisado no arquivo: ");
    scanf("%c",&c);
    buscar(c);
    return 0;
}
