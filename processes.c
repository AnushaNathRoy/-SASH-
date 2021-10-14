#include "headers.h"
extern char h[5000];
extern maindir[5000];
struct linked_list jobarr[500], pidfg;
int jobno = 1;

//completed background process signal
int completed_background(int signal_number)
{
    int cstatus;
    pid_t child_pid = waitpid(-1, &cstatus, WNOHANG);
    if (child_pid > 0)
    {
        for (int i = 0; i <= jobno; i++)
        {
            if (jobarr[i].lpid == child_pid)
            {
                fprintf(stderr, "\n%s with pid %d exited ", jobarr[i].command, jobarr[i].lpid);

                if (WIFEXITED(cstatus))
                {
                    fprintf(stderr, "normally\n");
                }
                else
                {
                    fprintf(stderr, "abnormally\n");
                }
            }
        }
    }
    return -1;
}

//execute forward
void execute_forward(char **args, int argno)
{
    int retvalue;
    int cstatus;
    retvalue = fork();
    char message[1000] = "";
    char temp[1000] = "";
    for (int i = 0; i < argno; i++)
    {
        sprintf(temp, " %s", args[i]);
        strcat(message, temp);
    }

    if (retvalue == 0)
    {
        //child
        int retvalue;
        args[argno] = NULL;
        retvalue = execvp(args[0], args);
        if (!(argno == 1 && strcmp(args[0], "") == 0))
            printf("Error: Could not run the command.\n");
    }
    if (retvalue != 0)
    {
        //parent
        pidfg.lpid = retvalue;
        strcpy(pidfg.command, message);
        waitpid(retvalue, &cstatus, WUNTRACED);
        pidfg.lpid = mainpid;
        strcpy(pidfg.command, "shell");

        return;
    }
    return;
}

//execute processes in background
void execute_backward(char **args, int argno)
{
    char message[1000] = "";
    char temp[1000] = "";
    for (int i = 0; i < argno; i++)
    {
        sprintf(temp, " %s", args[i]);
        strcat(message, temp);
    }
    int retvalue;
    retvalue = fork();

    if (retvalue == 0)
    {
        //child

        args[argno] = NULL;
        setpgid(0, 0);
        execvp(args[0], args);
        //jobno --;
        if (!(argno == 1 && strcmp(args[0], "") == 0))
        {
            printf("Error: Could not run the command.\n");
        }
    }
    if (retvalue != 0)
    {
        setpgid(retvalue, 0);
        fprintf(stderr, "%ld\n", retvalue);
        jobarr[jobno].lpid = retvalue;
        jobarr[jobno].job_no = jobno;
        strcpy(jobarr[jobno].command, message);
        jobno++;

        return;
    }
    return;
}

//run pinfo command
void execute_pinfo(char **args, int argno)
{
    int vpid;
    char spid[20];
    char file_path[2000] = "/proc/";
    char file_pathe[2000] = "/proc/";
    if (argno == 1 || (argno == 2 && strcmp(args[1], "") == 0))
    {
        vpid = getpid();
        printf("pid -- %d\n", vpid);
        sprintf(spid, "%d", vpid);
        strcat(file_path, spid);
        strcat(file_pathe, spid);
    }

    else
    {
        printf("\npid -- %s\n", args[1]);
        strcat(file_path, args[1]);
        strcat(file_pathe, args[1]);
    }

    strcat(file_path, "/stat");

    ll file_descriptor = open(file_path, O_RDONLY);

    if (file_descriptor < 0)
    {
        perror("Error:");
        return;
    }

    int sz = 500000;
    char *b = (char *)calloc(sz, sizeof(char));
    int rsz;
    rsz = read(file_descriptor, b, sz);
    b[rsz] = '\0';

    char *sarg[100];
    sarg[0] = strtok(b, " ");
    int i = 0;
    for (i = 1;; i++)
    {
        sarg[i] = strtok(NULL, " ");
        if (sarg[i] == NULL)
            break;
    }

    printf("Process Status -- %s", sarg[2]);
    int isnotforeground = (strcmp(sarg[7], sarg[0]));
    (isnotforeground) ? printf(" \n") : printf("+\n");
    printf("memory -- %s{Virtual Memory}\n", sarg[22]);

    printf("Executable Path -- ");
    strcat(file_pathe, "/exe");
    char epath[2000];
    int rete = readlink(file_pathe, epath, sizeof(epath));
    if (rete == 0)
    {
        printf("NULL\n");
    }
    else
    {
        epath[rete] = 0;
        strcpy(h, epath);

        int retvalue;
        retvalue = strstr(h, maindir);
        if (retvalue)
        {
            char output[500] = "";
            strncpy(output, h + strlen(maindir), strlen(h) - strlen(maindir));
            printf("~%s\n", output);
        }
        else
        {
            printf("%s\n", h);
        }
    }
}