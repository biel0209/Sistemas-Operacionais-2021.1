#include <dirent.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    /*
    FILE *arq = fopen("/usr/bin/time", "rb");
    char string[1000];
    if (arq == NULL)
        printf ("\n\nErro: Arquivo binario não foi aberto!\n");
    else{
        fread(string,sizeof(char),1000,arq);
    }
    printf ("%s\n",string);
    */
    system("\\time -f \"Tempo total: %e\nPorcentagem de uso da CPU: %P\nTempo em modo kernel: %S\nTempo em modo usuario: %U\nTrocas de contexto involuntarias: %c\nTrocas de contexto voluntarias: %w\n\" ./loop");

    return 0;
}
