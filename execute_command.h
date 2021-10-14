#ifndef _EXECUTE_H
#define _EXECUTE_H

void execute_command(char **args, int argno);
void parsing_command(char* ce_args1);
int parse_arguments(char *arg);
void execute_others(char **args, int argno);
void execute_repeat(char **args, int argno);
void execute_replay(char **args, int argno);
void prompt();
long long power(long long n, long long e);

#endif