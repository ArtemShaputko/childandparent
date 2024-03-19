int strcmpCollate(const void* first, const void* second);
char** copyArgv(char* argv[], int argc, int nAlloc);
void sortAndOutEnvp(char* envp[]);
void handleKeyboard(int argc, char* argv[], char* envp[]);
void executeChild(char c, int argc, int nAlloc, char ** newArgv, char *envp[], int counter);
char* findParamEnvp(char ** envp, char*param);
void parseEnv(const char* env, char** key, char** value);
void freeStringArray(char ** array, int n);
int getch();