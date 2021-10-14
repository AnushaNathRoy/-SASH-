#include "headers.h"
extern char h[5000];
extern maindir[5000];
void prompt()
{
    getcwd(h, 5000);
    int retvalue;
    retvalue = strstr(h, maindir);
    if (retvalue)
    {
        char output[500] = "";
        strncpy(output, h + strlen(maindir), strlen(h) - strlen(maindir));
        printf("~%s", output);
    }
    else
    {
        printf("%s", h);
    }
}
// a function to calculate the e-th power of a number n
long long power(long long n, long long e)
{
    long long answer = 1;
    for (long long i = 0; i < e; i++)
    {
        answer *= n;
    }
    return answer;
}

void execute_repeat(char **args, int argno)
{

    // printf("reapeat");

    if (argno <= 2)
    {
        printf("Error: No more commands after repeat.\n");
        return;
    }

    // char **newargs = NULL;
    char *newargs[500];
    int n = 0, repeati = 0, k = 0;
    char *argn = args[1];
    //get the number arg2 string -> long long
    for (int i = 0; i < strlen(argn); i++)
    {
        n = argn[strlen(argn) - 1 - i] - '0';
        repeati += n * power(10, i);
        // printf("repeati: %d\n",repeati);
    }
    // printf("repeati: %d\n",repeati);

    for (int j = 2; j < argno; k++, j++)
    {
        newargs[k] = malloc(strlen(args[j]) * sizeof(char));
        strcpy(newargs[k], args[j]);
    }

    for (int i = 0; i < repeati; i++)
    {
        execute_command(newargs, k);
    }

    for (int j = 2; j < argno; j++)
    {
        free(newargs[j - 2]);
    }
}
void execute_others(char **args, int argno)
{
    if (strcmp(args[argno - 1], "&") == 0)
    {
        argno--;
        //  printf("===>%d",argno);
        execute_backward(args, argno);
    }
    else
    {
        execute_forward(args, argno);
    }
}

int parse_arguments(char *arg)
{

    if (strcmp(arg, "-a") == 0)
        return 0;
    if (strcmp(arg, "-l") == 0)
        return 1;

    if (strcmp(arg, "..") == 0)
        return 2;
    if (strcmp(arg, ".") == 0)
        return 3;
    if (strcmp(arg, "~") == 0)
        return 4;
    if (strcmp(arg, "-al") == 0)
        return 5;
    if (strcmp(arg, "-la") == 0)
        return 6;
    if (strcmp(arg, "") == 0)
        return 7;
    if (strcmp(arg, "-r") == 0)
        return 8;
    if (strcmp(arg, "-s") == 0)
        return 9;
    if (strcmp(arg, " ") == 0)
        return 10;
    if (strcmp(arg, "-sr") == 0)
        return 11;
    if (strcmp(arg, "-rs") == 0)
        return 12;
    if (strcmp(arg, "-command") == 0)
        return 14;
    if (strcmp(arg, "-interval") == 0)
        return 15;
    if (strcmp(arg, "-period") == 0)
        return 16;
    return -1;
}

void execute_command(char **args, int argno)
{
    int len = strlen(args[argno - 1]);
    if (args[argno - 1][len - 1] == '\n')
    {
        args[argno - 1][len - 1] = '\0';
        len--;
    }
    if (args[argno - 1][len - 1] == ';')
    {
        args[argno - 1][len - 1] = '\0';
    }
    if (args[argno - 1] == NULL)
    {
        argno--;
    }
    // }
    if (strcmp(args[0], "cd") == 0)
        execute_cd(args, argno);
    else if (strcmp(args[0], "echo") == 0)
        execute_echo(args, argno);
    else if (strcmp(args[0], "pwd") == 0)
        execute_pwd(args, argno);
    else if (strcmp(args[0], "ls") == 0)
        execute_ls(args, argno);
    else if (strcmp(args[0], "repeat") == 0)
        execute_repeat(args, argno);
    else if (strcmp(args[0], "replay") == 0)
        execute_replay(args, argno);
    else if (strcmp(args[0], "pinfo") == 0)
        execute_pinfo(args, argno);
    else if (strcmp(args[0], "exit") == 0)
        exit(0);
    else if (strcmp(args[0], "history") == 0)
        displayHistory(args, argno);
    else if (strcmp(args[0], "jobs") == 0)
        execute_jobs(args, argno);
    else if (strcmp(args[0], "sig") == 0)
        execute_sg(args, argno);
    else if (strcmp(args[0], "fg") == 0)
        execute_fg(args, argno);
    else if (strcmp(args[0], "bg") == 0)
        execute_bg(args, argno);
    else
        execute_others(args, argno);
}

void execute_replay(char **args, int argno)
{

    int command_arg_count = 0, command_arg_start = 0;
    int valid_arg_count = 0;
    char *newargs[500], *argn;
    int n = 0, repeati = 0, k = 0, interval, period;

    if (argno <= 6)
    {
        printf("Error: replay: Invalid arguments.\n");
        return;
    }

    // int len = strlen(args[argno - 1]);
    // if (args[argno - 1][len - 1] == ';' || args[argno - 1][len - 1] == '\n')
    // {
    //     args[argno - 1][len - 1] = '\0';
    // }
    // if (strcmp(args[argno - 1], "")){
    //     argno --;
    // }

    for (int l = 1; l < argno; l++)
    {
        switch (parse_arguments(args[l]))
        {
        case 14:
            command_arg_start = l + 1;
            valid_arg_count++;
            break;

        case 15:
            argn = args[l + 1];
            n = 0; interval=0;
            //get the number arg2 string -> long long
            for (int i = 0; i < strlen(argn); i++)
            {
                n = argn[strlen(argn) - 1 - i] - '0';
                interval += n * power(10, i);
                // printf("repeati: %d\n",repeati);
            }
            l++;
            valid_arg_count++;
            break;

        case 16:
            n = 0; period=0;
            argn = args[l + 1];
            //get the number arg2 string -> long long
            for (int i = 0; i < strlen(argn); i++)
            {
                n = argn[strlen(argn) - 1 - i] - '0';
                period += n * power(10, i);
                // printf("repeati: %d\n",repeati);
            }
            l++;
            valid_arg_count++;
            break;

        default:
            command_arg_count++;
            break;
        }
    }

    for (int j = command_arg_start; j < command_arg_start + command_arg_count; k++, j++)
    {
        //fprintf(stderr,"j: %d  k:%d\n",j,k );
        newargs[k] = malloc(strlen(args[j]) * sizeof(char));
        strcpy(newargs[k], args[j]);
    }

    newargs[k] = NULL;

    // for(int pi=0; pi<k; pi++){
    //     fprintf(stderr,"ll: %s\n", newargs[pi]);
    // }

    repeati = period/interval;
    //fprintf(stderr,"s: %d  cn: %d  p: %d  i: %d\n", command_arg_start, command_arg_count, period, interval);
    for (int i = 0; i < repeati; i++)
    {
        sleep(interval);
        execute_command(newargs, k);
        
    }

    for (int j = 2; j < argno; j++)
    {
        free(newargs[j - 2]);
    }
}