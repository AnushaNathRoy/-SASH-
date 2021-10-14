#ifndef _PRO_H
#define _PRO_H

void execute_forward(char **args, int argno);
void execute_backward(char **args, int argno);
void execute_pinfo(char **args, int argno);
int completed_background(int signal_number);

struct linked_list
{
    int lpid;
    int job_no;
    char command[500];
};
extern struct linked_list jobarr[500], pidfg;



#endif