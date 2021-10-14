#include "headers.h"
extern jobno;

// a helper function for qsort on alaphabetical order of command
int qsort_cmp(const void *a, const void *b)
{
    struct linked_list *ia = (struct linked_list *)a;
    struct linked_list *ib = (struct linked_list *)b;
    return strcmp(ia->command, ib->command);
}

//execute signal command
void execute_sg(char **args, int argno)
{
    int ip_signal = 0, n = 0, ip_jobid = 0;

    for (int i = 0; i < strlen(args[1]); i++)
    {
        n = args[1][strlen(args[1]) - 1 - i] - '0';
        ip_jobid += n * power(10, i);
    }
    for (int i = 0; i < strlen(args[2]); i++)
    {
        n = args[2][strlen(args[2]) - 1 - i] - '0';
        ip_signal += n * power(10, i);
    }

    //printf("ip_jobid: %d, ip_signal: %d \n", ip_jobid, ip_signal);
    if (ip_jobid >= jobno || ip_jobid < 0)
    {
        fprintf(stderr, "Error: No job with the given job number exists\n");
        return;
    }
    int ip_pid = jobarr[ip_jobid].lpid;
    int retvalue = kill(ip_pid, 0);
    if (retvalue == -1)
    {
        fprintf(stderr, "Error: No job with the given job number exists\n");
        return;
    }

    retvalue = kill(ip_pid, ip_signal);
    if (retvalue == -1)
    {
        fprintf(stderr, "Error: signal cannot be sent\n");
        return;
    }
}

//execute jobs command
void execute_jobs(char **args, int argno)
{
    int r = 0, s = 0;
    int jobarr_length = jobno;
    //printf("inside jobs\n");
    struct linked_list sorted_jobarr[500];
    for (int i = 0; i < jobarr_length; i++)
    {
        sorted_jobarr[i] = jobarr[i];
        //printf("[%d] %s [%d]",sorted_jobarr[i].job_no, sorted_jobarr[i].command, sorted_jobarr[i].lpid);
    }

    for (int i = 1; i < argno; i++)
    {
        // printf("\nargno: <%s>\n", args[i]);
        if (i == argno - 1)
        {
            int len = strlen(args[argno - 1]);
            if (args[argno - 1][len - 1] == ';' || args[argno - 1][len - 1] == '\n')
            {
                args[argno - 1][len - 1] = '\0';
            }

            if (strcmp(args[argno - 1], " ") == 0 || strcmp(args[argno - 1], "") == 0)
            {
                break;
            }
        }

        char retvalue = parse_arguments(args[i]);

        switch (retvalue)
        {
        case 8:
            r = 1;
            break;
        case 9:
            s = 1;
            break;
        case 10:
            break;
        case 11:
        case 12:
            r = 1;
            s = 1;
            break;
        default:
            fprintf(stderr, "invalid arguments\n");
            break;
        }
    }

    qsort(sorted_jobarr, jobarr_length, sizeof(struct linked_list), qsort_cmp);
    int offset = 0;
    for (int i = 0; i < jobarr_length; i++)
    {
        char file_path[2000];
        sprintf(file_path, "/proc/%d/stat", sorted_jobarr[i].lpid);
        //printf("%s", file_path);

        ll file_descriptor = open(file_path, O_RDONLY);

        if (file_descriptor < 0)
        {
            //perror("Error:");
            continue;
        }

        int sz = 500000;
        char *b = (char *)calloc(sz, sizeof(char));
        int rsz;
        rsz = read(file_descriptor, b, sz);
        b[rsz] = '\0';

        char *sarg[100];
        sarg[0] = strtok(b, " ");
        int k = 0;
        for (k = 1;; k++)
        {
            sarg[k] = strtok(NULL, " ");
            if (sarg[k] == NULL)
                break;
        }
        char status[50];
        if (strcmp(sarg[2], "T") == 0)
            strcpy(status, "Stopped");
        else
            strcpy(status, "Running");
        // else if(strcmp(sarg[2], "T")==0) strcpy(status, "Stopped");
        // else if(strcmp(sarg[2], "S")==0) strcpy(status, "Sleeping in an interruptible wait");
        // else if(strcmp(sarg[2], "Z")==0) strcpy(status, "Zombie");
        //printf("Process Status -- %s", sarg[2]);
        if (r & s)
        {
            printf("[%d] %s %s [%d]\n", sorted_jobarr[i].job_no, status, sorted_jobarr[i].command, sorted_jobarr[i].lpid);
            continue;
        }
        if (r)
        {
            if (strcmp(status, "Running") != 0)
                continue;
        }

        if (s)
        {
            if (strcmp(status, "Stopped") != 0)
                continue;
        }
        printf("[%d] %s %s [%d]\n", sorted_jobarr[i].job_no, status, sorted_jobarr[i].command, sorted_jobarr[i].lpid);
    }
}

//execute fg command
void execute_fg(char **args, int argno)
{
    int ip_jobid = 0, n = 0, status;

    if (argno == 1)
    {
        fprintf(stderr, "Error: Arguments\n");
        return;
    }

    for (int i = 0; i < strlen(args[1]); i++)
    {
        n = args[1][strlen(args[1]) - 1 - i] - '0';
        ip_jobid += n * power(10, i);
    }

    if (ip_jobid >= jobno || ip_jobid <= 0)
    {
        fprintf(stderr, "Error: No job with the given job number exists\n");
        return;
    }
    int ip_pid = jobarr[ip_jobid].lpid;
    int retvalue = kill(ip_pid, 0);
    if (retvalue == -1)
    {
        fprintf(stderr, "Error: No job with the given job number exists\n");
        return;
    }

    int retval, cstatus;
    jobarr[ip_jobid].lpid = -1;
    pidfg.lpid = ip_pid;
    //printf("pid in efd: %d", ip_pid);
    strcpy(pidfg.command, jobarr[ip_jobid].command);
    retval = kill(ip_pid, SIGCONT);
    if (retval == -1)
    {
        fprintf(stderr, "error\n");
        return;
    }

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    tcsetpgrp(0, getpgid(ip_pid));

    waitpid(ip_pid, &cstatus, WUNTRACED);

    tcsetpgrp(0, mainpid);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);

    pidfg.lpid = mainpid;
    strcpy(pidfg.command, "shell");
}

//execute bg command
void execute_bg(char **args, int argno)
{
    int ip_jobid = 0, n = 0, status;

    if (argno == 1)
    {
        fprintf(stderr, "Error: Arguments\n");
        return;
    }

    for (int i = 0; i < strlen(args[1]); i++)
    {
        n = args[1][strlen(args[1]) - 1 - i] - '0';
        ip_jobid += n * power(10, i);
    }

    if (ip_jobid >= jobno || ip_jobid <= 0)
    {
        fprintf(stderr, "Error: No job with the given job number exists\n");
        return;
    }
    int ip_pid = jobarr[ip_jobid].lpid;
    int retvalue = kill(ip_pid, 0);
    if (retvalue == -1)
    {
        fprintf(stderr, "Error: No job with the given job number exists\n");
        return;
    }

    retvalue = kill(ip_pid, SIGCONT);
    if (retvalue == -1)
    {
        fprintf(stderr, "Error: signal cannot be sent\n");
        return;
    }
}