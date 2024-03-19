#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "func.h"


void main(int argc, char* argv[], char* envp[])
{
    sortAndOutEnvp(envp);
    handleKeyboard(argc,argv,envp);
}