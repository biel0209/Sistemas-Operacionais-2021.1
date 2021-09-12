#include <dirent.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

asdas
int isProcessDir (const struct dirent *namelist) {
    if (isdigit(namelist->d_name[0]) == 0)
        return 0;
    else
        return 1;
}

void lerNome(char *dir_name, char *pid)
{
    FILE *arq;
    char name_process[50];
    sprintf(dir_name, "/proc/%s/comm",pid);
    arq = fopen(dir_name,"r");
    fgets(name_process, 50, arq);
    name_process[strcspn(name_process, "\n")] = 0;
    printf("%20s\t\t", name_process);
}

void quebrarStatus(char *status)
{
    int tam = strlen(status) - 6;
    char stat[tam];
    memcpy(stat, &status[7],tam);
    stat[strcspn(stat, "\n")] = 0;
    printf("%20s",stat);
}

void lerStatus(char *dir_name, char *pid)
{
    FILE *arq;
    char status[50];
    sprintf(dir_name, "/proc/%s/status",pid);
    arq = fopen(dir_name,"r");
    int i=1;
    while(fgets(status, 50, arq) != NULL && i <= 3){
        if (i == 3)
            quebrarStatus(status);
        i++; 
    }
}

int main()
{
    srand(time(NULL));
    struct dirent **namelist;
    int n;
    n = scandir ("/proc", &namelist,isProcessDir, NULL);
    char dir_name[500];
    printf("\t\t   PID\t\t   Process\t\t\t  Status\t\t\t  IdUser\n");
    for(int i=0; i<n; i++){ 
        printf("%20s\t", namelist[i]->d_name);
        lerNome(dir_name,namelist[i]->d_name);
        lerStatus(dir_name,namelist[i]->d_name); 
        printf("\t%20d\n", rand() % 3);
    }
    return 0;
}
