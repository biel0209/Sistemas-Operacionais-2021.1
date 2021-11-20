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
<<<<<<< HEAD
        cont++;
      //printf("%c" , c);        
=======
        cont++;       
>>>>>>> 43349bb3840f3c2a2dfbc06d9f8a832afcc7abe0
    }while (x != EOF);
    printf("Ocorrencias de '%c' no arquivo: %d\n" , c, cont);        
}

/*A funcao principal escaneia uma entrada dada pelo usuario e
envia para funcao buscar.*/
int main()
{
    char c;
<<<<<<< HEAD
    printf("Digite um caracter a ser pesquisado no arquivo: ");        
=======
    printf("Digite um caracter a ser pesquisado no arquivo: ");
>>>>>>> 43349bb3840f3c2a2dfbc06d9f8a832afcc7abe0
    scanf("%c",&c);
    buscar(c);
    return 0;
}
