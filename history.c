#include "headers.h"
extern maindir[5000];
char *history[2000];
int hisi = 0;


void addHistory(char* command){

char temp[200];
strncpy(temp,command,strlen(command)-1);
temp[strlen(command)-1]='\0';

if(hisi!=0){
    if((strcmp(temp,history[hisi-1])==0) || (strcmp(temp," ")==0)){return;}
}

history[hisi] = malloc(sizeof(char)*200);
strncpy(history[hisi],command,strlen(command)-1);
hisi++;
char output_path[500];
sprintf(output_path,"%s/history.txt",maindir);
ll output_descriptor = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (output_descriptor < 0)
    {
        perror("w1");
        exit(1);
    }
    int i;
    if((hisi - 20)<0){i= 0;}
    else i= hisi - 20;
    for(;i<hisi;i++){
        char temp[100];
        sprintf(temp,"%s\n",history[i]);
        write(output_descriptor, temp, strlen(temp));
         
    }
     close(output_descriptor);
     return;

}

void loadHistory(){
         ll output_descriptor = open("history.txt", O_RDONLY, 0600);
          if (output_descriptor < 0)
          {
                perror("Error: could not open history.txt");
                
         }
         int sz = 20000;
         char *b = (char *)calloc(sz, sizeof(char));
         int rsz;
         rsz = read(output_descriptor, b, sz);
         b[rsz] = '\0';
         hisi=0;
         int i =0;
        history[i] = strtok(b,"\n");
        for (i = 1;; i++)
        {
            history[i] = strtok(NULL, "\n");
            if (history[i] == NULL)
                break;
        }
        hisi=i;
        if(history[0]==NULL){
            hisi=0;
        }
        close(output_descriptor);
         
}

void historyNum(){
    
}

void displayHistory(char **args, int argno){

    if(argno == 1)
    {
        int i;
        if((hisi - 20)<0){i= 0;}
        else i= hisi - 20;
        for(;i<hisi;i++){
            printf("%s\n",history[i]);
        }
    }
    else{
          int n=0,repeati=0,k=0,i=0;
        char *argn = args[1];

        for (int i = 0; i < strlen(argn); i++)
        {
            n = argn[strlen(argn) - 1 - i] - '0';
            repeati += n * power(10, i);
       
        }

        if((hisi - repeati)<0){i=0;}
        else i = hisi-repeati;
        for(;i<hisi;i++){
            printf("%s\n",history[i]);
        }

    }
}