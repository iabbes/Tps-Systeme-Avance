#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h> //pour avoir la var errno et perror
#include <sys/stat.h> //pour open
#include <fcntl.h> // pour open 
#include <unistd.h> // pour read and write


int main(int argc, char const *argv[]){
    // verifier qu'il y a tous les arguments 
    if ( argc != 3 ){   
        perror("Argument(s) manquant(s) --> 3 arguments");
        exit (EXIT_FAILURE);
    }

    int src = open (argv[1], O_RDONLY);

    if (src == -1){
        perror("pb open src");
        exit (EXIT_FAILURE);
    }
    
    int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (dest == -1){
        perror("pb open dest");
        exit (EXIT_FAILURE);
    }

    char c;
    int r;
    int w;

    //tant qu'il n'a pas fini de read il continue à stocker les données dans c

    while ((r = read (src, &c, 1)) > 0){
        

        // il écrit dans le 2eme fichier

        w = write (dest, &c, 1);

        if (w == -1){
            perror("pb write dans dest");
            exit(EXIT_FAILURE);
        }

    }

    if (r == -1){
        perror("pb read src");
        exit(EXIT_FAILURE);
    }

    int closeSrc = close(src);

    if (closeSrc == -1){
        perror("pb close src");
        exit(EXIT_FAILURE);
    }

    int closeDest = close(dest);

    if (closeDest == -1){
        perror("pb close dest");
        exit(EXIT_FAILURE);
    }

    return 0;
}
