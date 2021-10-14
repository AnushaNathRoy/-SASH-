#ifndef _HEADERS_H
#define _HEADERS_H


#include <pwd.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#define ll long long int
#include "headers.h"
#include "redirection.h"
#include "history.h"
#include "processes.h"
#include "built_in_command.h"
#include "execute_command.h"
#include "signals.h"
#include "process_manupilation.h"

#include <sys/types.h>
#include <dirent.h>
#include <grp.h>
void parsing_command(char *ce_args1);
void redirection_parse(char pcommand[500]);
int pipe_parse(char pcommand[500]);
void end_parsing_command();
extern int mainpid;
// required in MacOS
// #include <libproc.h>

#endif