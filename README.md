# README.md

**OS Assignment-3 Anusha Nath Roy (2020101124)**

---

# < SASH />

## ~$> Welcome to your new and optimised shell sash!  —

---

# Getting Started:

---

Download the sash shell file and cd into the directory

Run the command:

```bash
make -f makefile;
./a.out;
```

Now your sash is ready to go! The commands available at your tips and their execution examples is given below.

# Commands and Sample Execution :

---

**Input command types supported:**

- Commands with an arbitrary number of spaces will run without problem. No more worrying about whether spacing is right anymore!
    
    ```bash
    :~> ls -l    -a
    total 392
    -rw-r--r--	1	root	root	129	Oct 07 20:37	Makefile
    -rw-r--r--	1	root	root	4859	Oct 10 16:38	README.md
    -rwxr-xr-x	1	root	root	78216	Oct 10 16:32	a.out
    drwxr-xr-x	3	root	root	96	Oct 05 11:11	a.out.dSYM
    -rw-r--r--	1	root	root	685	Oct 10 16:37	bugs.md
    -rw-r--r--	1	root	root	8509	Oct 05 12:50	built_in_command.c
    -rw-r--r--	1	root	root	296	Sep 19 10:05	built_in_command.h
    -rw-r--r--	1	root	root	6435	Oct 10 16:43	execute_command.c
    -rw-r--r--	1	root	root	355	Oct 10 10:58	execute_command.h
    -rw-r--r--	1	root	root	762	Oct 10 09:38	headers.h
    -rw-r--r--	1	root	root	2281	Sep 19 20:34	history.c
    -rw-r--r--	1	root	root	154	Sep 19 19:54	history.h
    -rw-------	1	root	root	857	Oct 10 17:59	history.txt
    -rw-r--r--	1	root	root	5791	Oct 10 16:20	main.c
    -rw-r--r--	1	root	root	1367	Oct 10 16:50	output.txt
    -rw-r--r--	1	root	root	6548	Oct 10 16:32	process_manupilation.c
    -rw-r--r--	1	root	root	206	Oct 07 17:52	process_manupilation.h
    -rw-r--r--	1	root	root	4347	Oct 10 16:35	processes.c
    -rw-r--r--	1	root	root	346	Oct 10 09:39	processes.h
    -rw-r--r--	1	root	root	3837	Oct 10 12:55	redirection.c
    -rw-r--r--	1	root	root	147	Oct 06 19:29	redirection.h
    -rw-r--r--	1	root	root	1145	Oct 10 16:21	signals.c
    -rw-r--r--	1	root	root	155	Oct 08 00:56	signals.h
    -rw-r--r--	1	root	root	1367	Oct 10 16:50	sorted.txt
    -rw-r--r--	1	root	root	1365	Oct 10 16:46	wc
    drwx------	27	root	root	864	Oct 10 16:50	.
    drwxr-xr-x	6	root	root	192	Oct 10 16:40	..
    ```
    

- Sash supports multiple commands on the same line separated by ";" easily.
    
    ```bash
    :~> echo hie; ls -a
    hie
    Makefile	README.md	a.out	a.out.dSYM	bugs.md	built_in_command.c	built_in_command.h	execute_command.c	execute_command.h	headers.h	history.c	history.h	history.txt	main.c	output.txt	process_manupilation.c	process_manupilation.h	processes.c	processes.h	redirection.c	redirection.h	signals.c	signals.h	sorted.txt	wc	.	..
    ```
    

- Sash supports input-output redirection with appending too.
    
    ```bash
    :~>sort > sorted.txt < output.txt
    :~>echo hieeee > o.txt
    :~>cat o.txt
    hieeee
    :~>echo hieee2 >> o.txt
    :~>cat o.txt
    hieeee
    hieee2
    :~>echo hienew > o.txt
    :~>cat o.txt
    hienew
    ```
    

- Sash supports piping too which integrates seamlessly with input - output redirection.
    
    ```bash
    :~>ls - la | wc > output.txt
    :~>cat output.txt
          6       8      59
    ```
    

**Special commands available to manage all you sash jobs easily at one swipe:**

- **jobs:**
    
    allows you to view the currently running background processes sorted alphabetically on command name with their status and their pid. You can run it with options "-r" and "-s" options too to filter out the running and stopped processes!
    
    ```bash
    :~>sleep 40 &
    73
    :~>vim &
    74
    :~>sleep 2 &
    75
    :~>jobs
    [3] Running  sleep 2 [75]
    [1] Running  sleep 40 [73]
    [2] Stopped  vim [74]
    :~>jobs
    [4] Running  sleep 2000 [77]
    [5] Running  sleep 70000 [78]
    [2] Stopped  vim [74]
    :~>jobs -r
    [4] Running  sleep 2000 [77]
    [5] Running  sleep 70000 [78]
    :~>jobs -s
    [2] Stopped  vim [74]
    ```
    

- **sig:**
    
    allows you to send a signal to the job number specified.
    
    ```bash
    :~>jobs -r
    [4] Running  sleep 2000 [77]
    [5] Running  sleep 70000 [78]
    
    :~>sig 5 9
     sleep 70000 with pid 78 exited abnormally
    ```
    

- **fg:**
    
    allows you to run a background process in the foreground.
    
    ```bash
    :~>jobs
    [7] Running  sleep 200000 [95]
    :~>fg 7
    ^C
    
    [8] Running  sleep 40000 [96]
    :~>fg 8
    ^Z
    :~>jobs
    [9] Stopped  sleep 40000 [96]
    :~>fg 9
    //sleeps here
    ```
    

- **bg:**
    
    allows you to run a stopped background process in the background.
    
    ```bash
    :~>sleep 40000
    ^Z
    :~>jobs
    [8] Stopped  sleep 40000 [96]
    :~>bg 8
    :~>jobs
    [8] Running  sleep 40000 [96]
    
    ```
    

**Special signal handers are provided too to give easy shortcut:**

- **ctrl z:**
    
    allows you to stop any foreground process and make it a stopped background.
    
    ```bash
    :~>fg 9
    ^Z
    :~>jobs
    [10] Stopped  sleep 40000 [96]
    ```
    

- **ctrl c:**
    
    allows you to stop any foreground process.
    
    ```bash
    :~>sleep 20000
    ^C
    :~>
    ```
    

- **ctrl d:**
    
    allows you to exit the sash shell. come visit us soon!
    

# Working and Contents:

---

**The assignment contains the following files:**

- main.c
- headers.h
- history.c
- history.h
- processes.c
- processes.h
- execute_command.c
- execute_command.h
- built_in_command.c
- built_in_command.h
- process_manupilation.c
- process_manupilation.h
- redirection.c
- redirection.h
- signals.c
- signals.h

---

## execute_command.c

## void execute_command(char **args, int argno):

This takes in the command-line arguments and the number of arguments as input. This figures out which function to call and what command to run depending upon the command line arguments.

## int parse_arguments(char *arg):

Takes in an argument and parses it to understand if it is a flag or not and return appropriate number signalling the nature of the argument.

## void execute_others(char **args, int argno):

This takes in the command-line arguments and the number of arguments as input.This executes commands which don’t need to be implemented by us. It also figures out whether the process should be run in background or not.

## void prompt():

This figures out the directory displayed in the prompt message of the shell. Displays ~/relative path or total absolute path depending upon location.

## long long power(long long n, long long e):

A utility function to calculate the e-th power of a number n.

## void execute_repeat(char **args, int argno):

This takes in the command-line arguments and the number of arguments as input. This executes the repeat feature where the command is repeated. It figures out how many times to repeat the command and what command and function to be repeated and calls appropriately.

## void execute_replay(char **args, int argno):

This takes the command and parses the command according to the option provided, i.e., either "-command", "-period" and "-interval". This replays the command given for the specified interval after every specified periodical time.

---

## built_in_command.c

## void execute_cd(char **args, int argno):

This takes in the command-line arguments and the number of arguments as input. This execute the built-in command cd.

## void execute_echo(char **args, int argno):

This takes in the command-line arguments and the number of arguments as input. This execute the built-in command echo.

## void execute_pwd(char **args, int argno):

This takes in the command-line arguments and the number of arguments as input. This execute the built-in command pwd.

## void execute_ls(char **args, int argno):

This takes in the command-line arguments and the number of arguments as input. This execute the built-in command ls. It also takes care of all the flags using utility function parse_arguments(). It also calls execute_lsl() to get all the details which is printed while running ls with flags -l -al and -la.

## void execute_lsl(char *entryname, char* entry):

This takes in the file pointer and name as input and prints all relevant information as required by ls -l command which as shown in man page is:

> If the -l option is given, the following information is displayed for each file: file mode, number of links, owner name, group name, number of bytes in the file, abbreviated month, day-of-month file was last modified, hour file last modified, minute file last modified, and the pathname. In addition, for each directory whose contents are displayed, the total number of 512-byte blocks used by the files in the directory is displayed on a line by itself, immediately before the information for the files in the directory. If the modification time of the file is more than 6 months in the past or future, then the year of the last modification is displayed in place of the hour and minute fields.
> 

## int calc_blksize(char* dire,int l, int a):

This is a utility function that calculates the “total number of 512-byte blocks used by the files in the directory” required by the ls command with -l .

---

## history.c

## void addHistory(char* command):

Adds the input command to the history and updates history.txt.

## void loadHistory():

Loads history from history.txt at beginning of the program.

## void displayHistory(char **args, int argno):

Runs history and history  commands.

---

## processes.c

## void execute_forward(char **args, int argno):

Runs forward processes where parent waits for the child to terminate.

## void execute_backward(char **args, int argno):

Runs backward processes where parent does not wait for the child to terminate.

## void execute_pinfo(char **args, int argno):

This takes in the command-line arguments and the number of arguments as input. This execute the pinfo command.

## int completed_background(int signal_number):

This in addition to signal(SIGCHLD, signal_handler) reports when a background process ends and its status.

---

## redirection.c

## int execute_re(char * ce_args1, int argno, char* re_args, char re_type):

Executes the redirection command on passed  in "re_args" and does appropriate action based on  "re_type" which could be either "<" , ">" or ">>" and returns -1 when it executes unsuccessfully.

## void parsing_command(char *ce_args1):

Runs and divides and parses the passed command "ce_args1" based on " ".

---

## process_manupilation.c

## void execute_jobs(char **args, int argno):

This takes in the arguments "args" and the number of arguments as input. This executes the command jobs. It shows the job number numbered from 1 by the shell along with its status "running" or "stopped". A non terminated process is considered "running". 

## void execute_sg(char **args, int argno):

This takes in the arguments "args" and the number of arguments as input. This executes the command sig and sends it to the appropriate job number.

## void execute_fg(char **args, int argno):

This takes in the arguments "args" and the number of arguments as input. This executes the command fg and sends it to the appropriate job number. Also does error handling.

## void execute_bg(char **args, int argno):

This takes in the arguments "args" and the number of arguments as input. This executes the command bg and sends it to the appropriate job number and make it running. Also does error handling.

---

## signals.c

## void ctrl_z_handler(int signal_number):

This executes the function on pressing ^z. It makes the current foreground process go to the background and stops it.

## void ctrl_c_handler(int signal_number):

This executes the function on pressing ^c. It stops the current foreground process.

## void ctrl_d_handler(int signal_number):

This executes the function on pressing ^d. It exits the shell.

---

## main.c

Steps:

- So here first prompt is displayed
- Then input is taken and parsed
- The input is first parsed based on";"
- Then the divided arguments is parsed on "|" using pipe_parse(char pcommand[500])
- This is further parsed on ">" , "<" and ">>" in redirection_parse(char pcommand[500])
- Here depending on this the command is separated out and input and output is appropriately changed. The separated command is then run on execute_command()
- execute_command() is called to figure out what command to run and appropriate functions are called
- if execute_others() is called it figures out whether it is foreground or background and does appropriate function.
- Also includes signal(SIGCHLD, completed_background) which reports when a background process ends and its status.

---