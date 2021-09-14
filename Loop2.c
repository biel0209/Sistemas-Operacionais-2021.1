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
      printf("%c" , c);        
    }while (x != EOF);
    printf("\nOcorrencias de '%c' no arquivo: %d\n" , c, cont);        
}

int main()
{
    FILE *arq;
    int cont;
    char alfabeto[26] = "abcdefghijklmnopqrstuvwxyz";
    int i=0;
    while(i<26){
      buscar(alfabeto[i]);
      i++;
    }
    return 0;
}
