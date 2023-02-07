#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>    //pour avoir la var errno et perror
#include <sys/stat.h> //pour open
#include <fcntl.h>    // pour open
#include <unistd.h>   // pour read and write
#include <pwd.h>
#include <time.h>

void raler(char *msg)
{
    perror(msg);
    exit(1);
}

// afficher le premier mot passé sur la ligne de commande et le nom du programme sur la sortie standard
int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <mot>\n", argv[0]);
        exit(1);
    }
    printf("Premier argument saisi : %s\n", argv[1]);

    pid_t pid;
    int raison;
    int fd;

    switch (pid = fork())
    {

    case -1: // erreur : ne pas oublier
        raler(" fork ");
        break;

    case 0: /* le fils */
        printf("fils : pid fils =  %d \n", getpid());
        int returnClose = close(STDERR_FILENO); // fermer la sortie standard
        if (returnClose == -1)
        {
            raler("close");
        }

        static char template[] = "/tmp/proc-exerciceXXXXXX";
        char fname[25];
        strcpy(fname, template);

        fd = mkstemp(fname); // ouvrir un fichier temporaire
        if (fd == -1)
        {
            raler("mkstemp");
        }

        printf("fils : nom du fichier ouvert : %s, avec le descripteur : %d\n", fname, fd);
        execlp(argv[1], argv[1], (void *)0); // lancer le programme passé en paramètre

        exit(1); // cordon sanitaire

    default: /* le père */
        printf("père : pid père = %d\n", getpid());

        if (wait(&raison) == -1) // Attente d'un fils
            raler("wait");

        if (WIFEXITED(raison))
            printf("père : code de retour = %d\n", WEXITSTATUS(raison));

        else if (WIFSIGNALED(raison))
            printf("père : terminaison par un signal\n");

        else
            printf("père : autre raison\n");

        printf("père : That's all folks!\n");
        exit(0);
    }
    return 0;
}
