#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h> //pour avoir la var errno et perror
#include <sys/stat.h> //pour open
#include <fcntl.h> // pour open 
#include <unistd.h> // pour read and write
#include <pwd.h>
#include <time.h>

void raler (char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char const *argv[])
{    
    pid_t pid ;
    int raison;

    switch ( pid = fork ()){
            
    case -1 : // erreur : ne pas oublier
        raler ( " fork " );
        break;

    case 0 : /* le fils */
        printf("fils : pid fils =  %d \nfils : numero pid pere =  %d\n", getpid(), getppid());
        exit (1) ; // cordon sanitaire
    
    default : /* le père */
        printf("père : pid fils = %d\n", pid); // pid pcq fork renvoi l'id du fils c'est le meilleur moyen
        
        if (wait(&raison) == -1) //Attente d'un fils
            raler("wait");
            
        if (WIFEXITED(raison))
            printf("père : code de retour = %d\n", WEXITSTATUS(raison));
            
        else if (WIFSIGNALED(raison))
            printf("père : terminaison par un signal\n");
            
        else 
            printf("père : autre raison\n");
            
        exit(0);

    }


    return 0;
}
