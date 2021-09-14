#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void tratar_Classificar(char porcentagem[])
{
    int i = 0;
    while(porcentagem[i] != '%')
        i++;
    porcentagem[i] = ' ';
    if (atoi(porcentagem) >= 50)
        printf("Este processo é classificado como CPU-bound\n");
    else
        printf("Este processo é classificado como IO-bound\n");
}

void imprimirResultados(char *string)
{
    FILE *arq = fopen(string, "r");
    char txt[50];
    char cpu[5];
    while(fgets(txt, 50, arq) != NULL){
        printf("%s", txt);
        if (txt[0] == 'P'){
            int tam = strlen(txt) - 26;
            char porc[tam];
            memcpy(porc, &txt[27],tam);
            porc[strcspn(txt, "\n")] = 0;
            strcpy(cpu, porc);
        } 
    }
    tratar_Classificar(cpu);
}

int main()
{
    printf("Loop 1:\n");
    system("\\time -o resultado1.txt -f \"Tempo total: %e\nPorcentagem de uso da CPU: %P\nTempo em modo kernel: %S\nTempo em modo usuario: %U\nTrocas de contexto involuntarias: %c\nTrocas de contexto voluntarias: %w\n\" ./loop1");
    printf("Loop 2:\n");
    system("\\time -o resultado2.txt -f \"Tempo total: %e\nPorcentagem de uso da CPU: %P\nTempo em modo kernel: %S\nTempo em modo usuario: %U\nTrocas de contexto involuntarias: %c\nTrocas de contexto voluntarias: %w\n\" ./loop2");
    printf("\n");
    imprimirResultados("resultado1.txt");
    printf("\n");
    imprimirResultados("resultado2.txt");
    return 0;
}
