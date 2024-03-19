#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include "func.h"

#define BUFF 255

extern char ** __environ;

void outEnvFromFile(int argc, char *argv[], char *envp[])
{
    if(argc<3)
    {
        printf("No file\n");
        return;
    }
    FILE* file = fopen(argv[1], "r");
    char line[BUFF];
    char * value;
    while(fgets(line, BUFF, file))
    {
        line[strcspn(line, "\n")] = '\0';
        if(!strcmp(argv[argc-1], "+"))
        {
            value = getenv(line);
                if(!value) 
                printf("Nothing found for value (%s)\n", line);
            else 
                printf("found %s = %s\n", line, value);
        }
        else if(!strcmp(argv[argc-1], "*"))
        {
            value = findParamEnvp(envp, line);
            if(!value) 
                printf("Nothing found for value (%s)\n", line);
            else 
                printf("found %s = %s\n", line, value);
            free(value);
        }
        else if(!strcmp(argv[argc-1], "&"))
        {
            value = findParamEnvp(__environ, line);
            if(!value) 
                printf("Nothing found for value (%s)\n", line);
            else 
                printf("found %s = %s\n", line, value);
            free(value);
        }
        else
        {
            printf("Undefined option (%s)\n", argv[argc-1]);
            return;
        }
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
    }
    free(key);
    free(value);
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