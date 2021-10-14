#ifndef _PROM_H
#define _PROM_H

void execute_jobs(char **args, int argno);
void execute_sg(char **args, int argno);
void execute_fg(char **args, int argno);
void execute_bg(char **args, int argno);

#endif