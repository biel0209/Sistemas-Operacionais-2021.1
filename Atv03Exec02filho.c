

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main()
{

    printf("[CHILD]: PID %d, starts counting:\n",getpid());
    int i=1;
    while(i<=100){
        printf("[CHILD]: i = %d\n",i);
        i++;
    }
    return 0;
}