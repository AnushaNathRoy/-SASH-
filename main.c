#include "headers.h"
char h[5000];
char maindir[5000];
char prev_working_dir1[5000];
int mainpid;
extern int rredir;

char *trim(char *string_s)
{
    int string_l = strlen(string_s);
    while (isspace(string_s[string_l - 1]))
        --string_l;
    while (*string_s && isspace(*string_s))
        ++string_s, --string_l;
    return strndup(string_s, string_l);
}

//run command with >/<
void redirection_parse(char pcommand[500])
{
    //fprintf(stderr, "inside >< %s\n", pcommand);
    char *delimiters = "<>";
    int k = 0;
    char command_extras[500];
    strcpy(command_extras, pcommand);

    char *ce_args = strtok(command_extras, delimiters);
    char *ce_args1, delim = "";
    char *ce_args2;
    int res;
    ce_args1 = strdup(ce_args);
    res = execute_re(ce_args1, 0, ce_args, "");
    //fprintf(stderr, "|%c| .... %s .... %s %c\n", delim, ce_args1, ce_args, delim);
    while (ce_args && res != -1)
    {
        //fprintf(stderr, "|%c| .... %s .... %s\n", delim, ce_args, ce_args2);

        delim = pcommand[ce_args - command_extras + strlen(ce_args)];
        //printf("prev: %c\n", pcommand[(ce_args - command_extras + strlen(ce_args)) + 1]);
        if(pcommand[(ce_args - command_extras + strlen(ce_args)) + 1] == '>'){
            rredir = 1;
        }
        ce_args = strtok(NULL, delimiters);

       
        if (!(ce_args))
        {
            res = execute_re(ce_args1, 1, ce_args, delim);
        }
        else
        {
            ce_args2 = trim(ce_args);
            res = execute_re(ce_args1, 0, ce_args2, delim);
        }
    }
}

//run command with |
int pipe_parse(char pcommand[500])
{
    int infd, ofd;
    ofd = dup(STDOUT_FILENO);
    infd = dup(STDIN_FILENO);
    char *pipe_command[500];
    pipe_command[0] = strtok(pcommand, "|");
    //printf("ola<%s>\n", pipe_command[0]);
    int pipe_count = 1;
    for (int i = 1;; i++)
    {
        pipe_command[i] = strtok(NULL, "|");

        if (pipe_command[i] == NULL)
            break;

        pipe_count++;
    }

    // for (int p = 0; p < pipe_count; p++)
    // {
    //     printf("%d<%s>\n", p, pipe_command[p]);
    // }

    for (int p = 0; p < pipe_count; p++)
    {
        //fprintf(stderr, "<,,,<%d>,,,,>", p);
        //piping creating 2 ends => vir_files[0] -> read  vir_files[1] -> write
        int vir_files[2];

        if (pipe(vir_files) < 0)
        {
            perror("Could not pipe.");
            break;
        }

        //change output to the write end of the vir_file -> vir_file [1]
        if (vir_files[1] < 0 || vir_files[0] < 0)
        {
            perror("pipe error in opening virtual files:");
            end_parsing_command();
            return -1;
        }

        if (p == pipe_count - 1)
        {
            if (dup2(ofd, STDOUT_FILENO) < 0)
            {
                perror("pipe redirection error:");
                end_parsing_command();
                return -1;
            }

            close(ofd);
            ofd = -1;
        }
        else
        {
            if (dup2(vir_files[1], STDOUT_FILENO) < 0)
            {
                perror("pipe redirection error:");
                end_parsing_command();
                return -1;
            }
            if (vir_files[1] != STDOUT_FILENO)
                close(vir_files[1]);
        }

        //run command
        //fprintf(stderr, "%d<%s>\n", p, pipe_command[p]);
        redirection_parse(pipe_command[p]);

        //change input to the read end of the vir_file -> vir_file [0]
        if (p == pipe_count - 1)
        {
            if (dup2(infd, STDIN_FILENO) < 0)
            {
                perror("pipe redirection error:");
                end_parsing_command();
                return -1;
            }

            close(infd);
            infd = -1;
        }
        else
        {
            if (dup2(vir_files[0], STDIN_FILENO) < 0)
            {
                perror("pipe redirection error:");
                end_parsing_command();
                return -1;
            }

            if (vir_files[0] != STDIN_FILENO)
                close(vir_files[0]);
        }
    }
}

int main()
{
    printf(""
 "*     .----------------.  .----------------.  .----------------.  .----------------. \n"
 "*    | .--------------. || .--------------. || .--------------. || .--------------. | \n"
 "*    | |    _______   | || |      __      | || |    _______   | || |  ____  ____  | | \n"
 "*    | |   /  ___  |  | || |     /  \\     | || |   /  ___  |  | || | |_   ||   _| | | \n"
 "*    | |  |  (__ \\_|  | || |    / /\\ \\    | || |  |  (__ \\_|  | || |   | |__| |   | | \n"
 "*    | |   '.___`-.   | || |   / ____ \\   | || |   '.___`-.   | || |   |  __  |   | | \n"
 "*    | |  |`\\____) |  | || | _/ /    \\ \\_ | || |  |`\\____) |  | || |  _| |  | |_  | | \n"
 "*    | |  |_______.'  | || ||____|  |____|| || |  |_______.'  | || | |____||____| | | \n"
 "*    | |              | || |              | || |              | || |              | | \n"
 "*    | '--------------' || '--------------' || '--------------' || '--------------' | \n"
 "*     '----------------'  '----------------'  '----------------'  '----------------'  \n"
);
printf("\n");
//history command
    loadHistory();
    mainpid = getpid();
    //printf("main pid: %d\n", mainpid);
    pidfg.lpid = mainpid;
    char input_command[500];
    getcwd(maindir, 5000);
    char *hostname[500];
    strcpy(prev_working_dir1, maindir);

    //waiting for background process to terminate
    signal(SIGCHLD, completed_background);

    //ctrl z
    signal(SIGTSTP, ctrl_z_handler);

    //ctrl c
    signal(SIGINT, ctrl_c_handler);

    //ctrl d
    //signal(SIGCHLD, ctrl_d_handler);

    while (1)
    {

        //prompt printing
        gethostname(hostname, 50);
        struct passwd *p = getpwuid(getuid());
        printf("\n");
        printf("\033[0;35m");
        printf(" ⌬ SASH <%s@", p->pw_name);
        printf("%s:", hostname);
        prompt();
        printf("> ");
        printf("\033[0m");

        //get input command
        fflush(stdin);
        int retvalue=0;
        retvalue = fgets(input_command, 500, stdin);
        if(!retvalue) exit(0);
        addHistory(input_command);

        //divide command on ";"
        char *command[500];
        int commandcount = 0;
        command[0] = strtok(input_command, ";");
        commandcount++;
        for (int i = 1;; i++)
        {
            command[i] = strtok(NULL, ";");

            if (command[i] == NULL)
                break;
            commandcount++;
        }

        for (int j = 0; j < commandcount; j++)
        {
            int res = pipe_parse(command[j]);
            if (res == -1)
            {
                break;
            }
            //char *pipe_command[500];
            //pipe_command[0] = strtok(input_command, ";");
            // while()

            //redirection_parse(command[j]);
        }
    }
}