#include "headers.h"
int fd;
int ofd = -1;
int infd = -1;
int rredir = 0;

void end_parsing_command()
{
    if (ofd != -1)
    {
        dup2(ofd, STDOUT_FILENO);
        close(ofd);
        ofd = -1;
    }
    if (infd != -1)
    {
        dup2(infd, STDIN_FILENO);
        close(infd);
        infd = -1;
    }
}

//run and divide command on " "
void parsing_command(char *ce_args1)
{

    char *arg[100];
    arg[0] = strtok(ce_args1, " ");
    int k;
    for (k = 1;; k++)
    {
        arg[k] = strtok(NULL, " ");
        if (arg[k] == NULL)
            break;
    }
    execute_command(arg, k);
}

int execute_re(char *ce_args1, int argno, char *re_args, char re_type)
{

    if (argno == 0)
    {

        //to clean the argument (filename etc)
        int len = strlen(re_args);

        if (re_args[len - 1] == '\n' || re_args[len - 1] == ';')
        {
            re_args[len - 1] = '\0';
            len--;
        }

        switch (re_type)
        {
        
        //output redirection
        case '>':
            if (rredir)
            {
                fd = open(re_args, O_WRONLY | O_CREAT | O_APPEND, 0644);
                rredir = 0;
                if (fd < 0)
                {
                    perror("redirection error5:");
                    end_parsing_command();
                    return -1;
                }

                ofd = dup(STDOUT_FILENO);
                if (dup2(fd, STDOUT_FILENO) < 0)
                {
                    perror("redirection error6:");
                    end_parsing_command();
                    return -1;
                }
                if (fd != STDOUT_FILENO)
                    close(fd);
            }
            else
            {
                fd = open(re_args, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0)
                {
                    perror("redirection error1:");
                    end_parsing_command();
                    return -1;
                }

                ofd = dup(STDOUT_FILENO);
                if (dup2(fd, STDOUT_FILENO) < 0)
                {
                    perror("redirection error2:");
                    end_parsing_command();
                    return -1;
                }

                if (fd != STDOUT_FILENO)
                    close(fd);
            }
            break;

        //input redirection
        case '<':
            fd = open(re_args, O_RDONLY);

            if (fd < 0)
            {
                perror("redirection error3:");
                end_parsing_command();
                return -1;
            }
            infd = dup(STDIN_FILENO);
            if (dup2(fd, STDIN_FILENO) < 0)
            {
                perror("redirection error4:");
                end_parsing_command();
                return -1;
            }
            if (fd != STDIN_FILENO)
            {
                close(fd);
            }
            break;

        default:
            break;
        }

        return 0;
    }

    //append redirection
    if (rredir)
    {

        fd = open(re_args, O_WRONLY | O_CREAT | O_APPEND, 0644);
        rredir = 0;
        if (fd < 0)
        {
            perror("redirection error7:");
            end_parsing_command();
            return -1;
        }
        ofd = dup(STDOUT_FILENO);
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            perror("redirection error8:");
            end_parsing_command();
            return -1;
        }
        if (fd != STDOUT_FILENO)
            close(fd);
    }

    parsing_command(ce_args1);

    //restoring the original stdin and stdout
    if (ofd != -1)
    {
        dup2(ofd, STDOUT_FILENO);
        close(ofd);
        ofd = -1;
    }
    if (infd != -1)
    {
        dup2(infd, STDIN_FILENO);
        close(infd);
        infd = -1;
    }

    return 0;
}
