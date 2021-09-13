#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *arq;
    int n;
    char c;
    arq = fopen("teste.txt","w");   
    n = fputs("Arquivo teste para atividade de SO.\n",arq);
    if (n == EOF)
        printf("Erro ao gravar no arquivo!");
    fclose(arq);
    arq = fopen("teste.txt","r");   
    do{
      c = fgetc(arq);
      printf("%c" , c);        
    }while (c != EOF);
    fclose(arq);
    return 0;
}
