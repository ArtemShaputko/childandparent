#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <limits.h>
#include "func.h"

#define BUFF 255

extern char ** __environ;

int strcmpCollate(const void* first, const void* second) {
    const char* str1 = *(const char**)first;
    const char* str2 = *(const char**)second;
    return strcoll(str1, str2);
}

char** copyArgv(char* argv[], int argc, int nAlloc)
{
    char** newArgv = calloc(nAlloc, sizeof(char*));
    for(int i = 0; i < argc; i++) 
    {
        if(!argv[i]) break;
        newArgv[i] = calloc(BUFF,sizeof(char));
        strcpy(newArgv[i], argv[i]);
    }
    return newArgv;
}

void sortAndOutEnvp(char *envp[])
{
    int envc = 0;
    for(;envp[envc];envc++);
    qsort(envp, envc, sizeof(char*), strcmpCollate);
    for(int i = 0; envp[i]; i++) printf("%s\n", envp[i]);
}

void handleKeyboard(int argc, char *argv[], char *envp[])
{
    int c;
    int counter = 0;
    char childName[8];
    int proc;
    int nAlloc = argc+2;
    char** newArgv = copyArgv(argv, argc, nAlloc);
    newArgv[argc] = calloc(2,sizeof(char));
    while(1)
    {
        c = getch();
        if(c=='q') return;
        if(c != '+' && c != '*' && c != '&')
        {
            printf("Wrong command\n");
            continue;
        }
        proc = fork();
        if(!proc)
        {
            executeChild(c,argc,nAlloc,newArgv,envp, counter);
            return;
        }
        else if(proc<0)     
            printf("Can`t fork proccess\n");
        else counter++;
        if(counter>99) counter=0;   
    }
    freeStringArray(newArgv, nAlloc);
}

void executeChild(char c, int argc, int nAlloc, char ** newArgv, char *envp[], int counter)
{
    sprintf(newArgv[0], "child_%02d", counter);
    char* childPath;
    switch(c)
    {
        case '+':
            newArgv[argc][0] = '+';
            execve(getenv("CHILD_PATH"), newArgv, envp);
            break;
        case '*' :
            newArgv[argc][0] = '*';
            childPath = findParamEnvp(envp, "CHILD_PATH");
            execve(childPath, newArgv, envp);
            free(childPath);
            break;
        case '&':
            newArgv[argc][0] = '&';
            childPath = findParamEnvp(__environ, "CHILD_PATH");
            execve(childPath, newArgv, envp);
            free(childPath);
            break;
        default:
            printf("Something went wrong");
            break;
    }
}

char *findParamEnvp(char **envp, char*param)
{
    char* key = calloc(BUFF, sizeof(char));
    char* value = calloc(BUFF, sizeof(char));
    for(int i = 0; envp[i]; i++)
    {
        if(strstr(envp[i], param))
        {
            parseEnv(envp[i], &key, &value);
            if(!strcmp(key, param))
            {
                free(key);
                return value;
            }
        }
        free(key);
        free(value);
    }
    return NULL;
}

void parseEnv(const char *env, char **key, char **value)
{
    char* newEnv = calloc(BUFF, sizeof(char));
    strcpy(newEnv, env);
    char* buff = strtok(newEnv, " =");
    strcpy(*key, buff);
    buff = strtok(NULL, "\"");
    strcpy(*value, buff);
    free(newEnv);
}

void freeStringArray(char **array, int n)
{
    for(int i = 0; i<n; i++) free(array[i]);
    free(array);
}

int getch() 
{
    int ch;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
