#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h> //pour avoir la var errno et perror
#include <sys/stat.h> //pour open
#include <fcntl.h> // pour open 
#include <unistd.h> // pour read and write

int main(int argc, char const *argv[]){
    // verifier qu'il y a tous les arguments 
    if ( argc != 2){   
        perror("Argument(s) manquant(s) --> 2 arguments");
        exit (EXIT_FAILURE);
    }

    int i = 0;

    int desc = open (argv[1], O_RDONLY);

    if (desc == -1){
        perror("pb open src");
        exit (EXIT_FAILURE);
    }
    
    int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (dest == -1){
        perror("pb open dest");
        exit (EXIT_FAILURE);
    }


    if (desc >= 0 ) {

    // recupere le nb de caractère du fichier
    off_t input_size = lseek(desc, 0, SEEK_END);
    lseek(desc, 0, SEEK_SET);
    char* buffer = (char*)malloc(input_size);
    read(desc, buffer, input_size);

    for(i=input_size; i>=0; i--){
      dprintf(1,"%c", buffer[i]);

    }
    dprintf(1,"\n");  

    int closeDesc = close(desc);

    if (closeDesc == -1){
        perror("pb close dest");
        exit(EXIT_FAILURE);
    }

    return 0;
    }
}

