#include "headers.h"
extern char h[5000];
extern maindir[5000];
extern prev_working_dir1[5000];
char prev_working_dir2[5000];

void execute_cd(char **args, int argno)
{
    // printf("len: %d ===> arg[2]: %s\n",argno,args[2]);
    int len = strlen(args[argno - 1]);
    if (args[argno - 1][len - 1] == ';' || args[argno - 1][len - 1] == '\n')
    {
        args[argno - 1][len - 1] = '\0';
    }
    if (argno > 3 || (argno == 3 && strcmp(args[2], ";") == 0))
    {
        printf("Error: cd:  More number of arguments than required.\n");
        return;
    }

    if (argno == 1 || (argno == 2 && strcmp(args[1],"")==0) )
    {
        chdir(maindir);
        strcpy(prev_working_dir2,prev_working_dir1);
        strcpy(prev_working_dir1,maindir);
        //printf("\nprevwor: %s >>>>2 >>>>> %s\n", prev_working_dir1, prev_working_dir2);
        return;
    }
    int retvalue;
    if(strcmp(args[1], "-")==0){
        chdir(prev_working_dir2);
        prompt();
        printf("\n");
        strcpy(prev_working_dir2,prev_working_dir1);
        strcpy(prev_working_dir1,prev_working_dir2);
        
        //printf("\nprevwor: %s >>>>2 >>>>> %s\n", prev_working_dir1, prev_working_dir2);
        return;
        //getcwd(h, 5000);
        
    }
    else if(args[1][0] == '~'){
        
        char output[500]="";
        char output1[500]="";
        strncpy(output, args[1] + 1 ,strlen(args[1]));
        sprintf(output1, "%s%s", maindir, output);
        retvalue = chdir(output1);
        strcpy(prev_working_dir2,prev_working_dir1);
        getcwd(prev_working_dir1, 5000);
        return;
    }

    retvalue = chdir(args[1]);

    if (retvalue < 0)
    {
        printf("Error: cd:  No directory under current path.\n");
        return;
    }
    
     strcpy(prev_working_dir2,prev_working_dir1);
     getcwd(prev_working_dir1, 5000);
     //printf("\nprevwor: %s >>>>2 >>>>> %s\n", prev_working_dir1, prev_working_dir2);
}

void execute_echo(char **args, int argno)
{
    int len = strlen(args[argno - 1]);
    if (args[argno - 1][len - 1] == ';' || args[argno - 1][len - 1] == '\n')
    {
        args[argno - 1][len - 1] = '\0';
    }
    for (int i = 1; i < argno; i++)
    {
        printf("%s", args[i]);
    }
    printf("\n");
}

void execute_pwd(char **args, int argno)
{
    getcwd(h, 5000);
    printf("%s\n", h);
}

void execute_lsl(char *entryname, char *entry){
    struct stat s;
    unsigned int mode;
    stat(entryname, &s);
    mode = s.st_mode;
    (S_ISREG(mode))? printf("-") : printf("d");
    (mode & S_IRUSR)? printf("r"): printf("-");
    (mode & S_IWUSR)? printf("w"): printf("-");
    (mode & S_IXUSR)? printf("x"): printf("-");
    (mode & S_IRGRP)? printf("r"): printf("-");
    (mode & S_IWGRP)? printf("w"): printf("-");
    (mode & S_IXGRP)? printf("x"): printf("-");
    (mode & S_IROTH)? printf("r"): printf("-");
    (mode & S_IWOTH)? printf("w"): printf("-");
    (mode & S_IXOTH)? printf("x"): printf("-");
    printf("\t%d",s.st_nlink);
    struct passwd *p = getpwuid(s.st_uid);
    printf("\t%s", p->pw_name);

    struct group *g = getgrgid(s.st_gid);
    printf("\t%s", g->gr_name);
    
    printf("\t%d",s.st_size);
    char output[200];
   
    time_t present = time(0);
    struct tm timem;
    localtime_r(&s.st_mtime,&timem);
    int yearm = timem.tm_year;
    int monthm = timem.tm_mon;
    struct tm timep;
    localtime_r(&present,&timep);
    int yearp = timep.tm_year;
    int monthp = timep.tm_mon;
    int monthsub = monthp - monthm;
    // printf("\t sub: %d",monthsub);
    if(!( yearm==yearp && ((monthsub>=0 && monthsub > 6) || (monthsub<0 && monthsub < -6)) || yearm != yearp)){
        strftime(output, 700, "%b %d %H:%M",localtime(&s.st_mtime));
    }
    else{
        strftime(output, 700, "%b %d %Y",localtime(&s.st_mtime));
    }
    
    printf("\t%s",output);
    
    printf("\t%s",entry);
    printf("\n");

}

int calc_blksize(char* dire,int l, int a){
        DIR *d;
        d = opendir(dire);
        if (d == NULL)
        {
            return -1;
        }
        struct dirent *readp;
            readp = readdir(d);
            int totalsize = 0;
            while (1)
            {
                readp = readdir(d);
                if (readp == NULL)
                    break;
                char entryname[500];
                strcpy(entryname, readp->d_name);
                if (entryname[0] == '.')
                {
                    if (!a)
                        continue;
                }
                if (l)
                {
                    
                    // printf("ls==>\n");
                    char path[500];
                    strcpy(path,dire);
                    strcat(path,"/");
                    strcat(path,entryname);
                    struct stat s;
                    unsigned int mode;
                    stat(path, &s);
                    totalsize += s.st_blocks;  

                }
            }

            closedir(d);
            return totalsize;
}

void execute_ls(char **args, int argno)
{
    char directories[500][500];
    // // printf("hirr");
    int a = 0, l = 0, diri = 0;
    if (argno == 1)
    {
        getcwd(directories[diri], 5000);
        diri++;
    }
    else
    {
        // printf("\nargno: %d\n", argno);
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

                if (strcmp(args[argno - 1], " ") == 0)
                    break;
            }

            char retvalue = parse_arguments(args[i]);

            switch (retvalue)
            {
            case 0:
                a++;
                break;
            case 1:
                l++;
                break;
            case 5:
                a++;
                l++;
                break;
            case 6:
                a++;
                l++;
                break;
            case 4:
                strcpy(directories[diri], maindir);
                diri++;
                break;
            case 3:
                getcwd(directories[diri], 5000);
                diri++;
                break;
            case 2:
                getcwd(directories[diri], 5000);
                strcat(directories[diri], "/..");
                // strcat(dirtp,terminate);
                // strcpy(directories[diri],dirtp);
                // sprintf(directories[diri], "%s/..", dirtp);
                diri++;
                break;
            case -1:
                // printf("%s", args[i]);
                strcpy(directories[diri], args[i]);
                diri++;
                // printf("in here");
                break;

            default:
                break;
            }
        }
    }

    if (diri == 0)
    {
        getcwd(directories[diri], 5000);
        diri++;
    }
    // printf("\ndiri: %d", diri);
    for (int n = 0; n < diri; n++)
    {
        int totalsize =calc_blksize(directories[n],l,a);
        DIR *d;
        if(diri > 1) {printf("%s:\n", directories[n]);}
        d = opendir(directories[n]);
        if (d == NULL)
        {
            printf("\nError: Opening Directory\n");
        }
        else
        {
           
           if(l) {
            printf("total %d\n",totalsize);
            }
            struct dirent *readp;
            while (1)
            {
                readp = readdir(d);
                if (readp == NULL)
                    break;
                char entryname[500] = "";
                strcpy(entryname, readp->d_name);
                // printf("<%s>\t", entryname);
                if (entryname[0] == '.')
                {
                    if (!a)
                        continue;
                }
                if (l)
                {
                    printf("\033[0;34m");
                    // printf("ls==>\n");
                    char path[500];
                    strcpy(path,directories[n]);
                    strcat(path,"/");
                    strcat(path,entryname);
                    execute_lsl(path,entryname);
                    printf("\033[0m");

                }
                else
                {
                    printf("\033[0;36m");
                    printf("%s\t", entryname);
                    printf("\033[0m");
                }
            }

            closedir(d);
            printf("\n");
        }
    }
}
