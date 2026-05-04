
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* int main(int argc, char *argv[], char **envp)
{
    while(*envp != NULL)
    {
        printf("%s\n", *envp);
        envp++;
    }
    return 0;
} */







int main(int argc, char **argv, char **envp)
{
    char var_amb;
    while (( var_amb = getopt(argc, argv, "a::n:")) != -1)
    {
        switch (var_amb)
        {
        case 'a':
            printf("Todas as variaveis de ambiente: \n");
            for(int i = 0; envp[i] != NULL; i++)
            {
                printf("%s\n", envp[i]);
            }
        case 'n':
            printf("O nome das N primeiras variaveis de ambiente: \n");
            for(int i = 0; i < atoi(optarg) && envp[i] != NULL; i++)
            {
                printf("%s\n", envp[i]);
            }
            break;
            default:
                printf("DEU ERRO AQUI BIXO\n");
                return 1;
        }
    }
    return 0;
}










/* #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char next_option;
    while ((next_option = getopt(argc, argv, "a::b:")) != -1)
    {
        switch (next_option)
        {
        case 'a':
            printf("-a foi incluído, seu valor associado é: %s\n", optarg);
            break;
        case 'b':
            printf("-b foi incluído, seu valor associado é: %s\n", optarg);
            break;
        default:
            perror("%Argumentos: -a [Valor Op.] -b [Valor Ob.]\n");
            return 1;
        }
    }
    return 0;
} */