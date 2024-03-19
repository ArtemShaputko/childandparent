#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>
#include "func.h"

void main(int argc, char* argv[], char* envp[])
{
    printf("\tNEW PROCCESS\n");
    printf("Program name: %s\n", argv[0]);
    printf("Parent process pid: %d\n", getppid());
    printf("\tChild process pid: %d\n", getpid());
    outEnvFromFile(argc, argv, envp);
    printf("\n");
}