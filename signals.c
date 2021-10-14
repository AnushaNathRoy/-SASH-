#include "headers.h"
extern int jobno;

//ctrl_z_handler
void ctrl_z_handler(int signal_number)
{

     //fprintf(stderr, "ola z\n");
     if (pidfg.lpid == -1)
    {
        return;
    }
    else if (pidfg.lpid == mainpid)
    {
        //fprintf(stderr, "ola noz\n");
        return;
    }
    else
    {
        
        int retvalue = kill(pidfg.lpid, SIGCHLD);
        if (retvalue == -1)
        {
            fprintf(stderr, "Error: signal cannot be sent\n");
            return;
        }
        //fprintf(stderr, "ola pid: %d\n", pidfg.lpid);
        jobarr[jobno].lpid = pidfg.lpid;
        jobarr[jobno].job_no = jobno;
        strcpy(jobarr[jobno].command, pidfg.command);
        jobno++;
    }
    return;
}

//ctrl_c_handler
void ctrl_c_handler(int signal_number)
{
    if (pidfg.lpid == -1)
    {
        return;
    }
    else
    {
        int retvalue = kill(pidfg.lpid, SIGINT);
        if (retvalue == -1)
        {
            fprintf(stderr, "Error: signal cannot be sent\n");
            return;
        }
    }
    return;
}

//ctrl_d_handler
void ctrl_d_handler(int signal_number)
{
    printf("bye!");
    exit(0);
}