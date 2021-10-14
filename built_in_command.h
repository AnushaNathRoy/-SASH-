#ifndef _BIC_H
#define _BIC_H

void execute_cd(char **args, int argno);
void execute_echo(char **args, int argno);
void execute_pwd(char **args, int argno);
void execute_ls(char **args, int argno);
void execute_lsl(char *entryname, char *entry);
int calc_blksize(char* dire,int l, int a);

#endif