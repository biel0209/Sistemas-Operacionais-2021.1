#include <stdio.h>
#include <stdlib.h>

 /*Neste programa eh declarado um ponteiro do tipo FILE que
apontara para o arquivo teste.txt atraves da funcao fopen. O
arquivo aberto eh do tipo w, pois atraves de fputs sera
gravada uma string como conteudo. Ha um tratamento para caso
haja algum erro no momento da gravacao. Apos isso o arquivo
eh fechado e reaberto novamente como do tipo r (somente
leitura).*/
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
