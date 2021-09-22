#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
      //printf("%c" , c);        
    }while (x != EOF);
    printf("Ocorrencias de '%c' no arquivo: %d\n" , c, cont);        
}

int main()
{
    char c;
    printf("Digite um caracter a ser pesquisado no arquivo: ");        
    scanf("%c",&c);
    buscar(c);
    return 0;
}
