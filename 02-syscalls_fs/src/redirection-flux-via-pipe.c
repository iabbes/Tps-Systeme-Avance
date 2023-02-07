#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void raler(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char const *argv[])
{
    int statutExec1, statutExec2;
	int p[2];
	pid_t pid;

	// Création du pipe (et vérification qu'il n'y a pas d'erreur lors de la création)
	if(pipe(p) < 0)
		raler("pipe");
	
	switch (pid = fork())
	{
	case -1: // erreur : ne pas oublier
        raler(" fork ");
        break;
	
	case 0: /* le fils */
		// on ferme la sortie du pipe fils et on redirige l'entrée du pipe sur l'entrée standard
		close(p[0]);
		dup2(p[1], 1);
		

		// Execute la commande ps eaux
		execlp ("ps", "ps", "eaux", (void*)0);

		// On récupère les erreurs
		perror("Erreur PS : ");
		close(p[1]);
		exit(1);
	
	default:
		break;
	}

	switch (pid = fork())
	{
	case -1: // erreur : ne pas oublier
        raler(" fork ");
        break;
	
	case 0: /* le fils */
		close(p[1]);
		dup2(p[0], 0);

		// Création du fichier dans /dev/null
		int fd = open("/dev/null", O_WRONLY);
		dup2(fd, 1);

		// Execute la commande grep root
		execlp ("grep", "grep", "^root", (void*)0);

		// Récupère les erreurs
		perror("Erreur GREP : ");
		exit(1);
	
	default:
		break;
	}

	close(p[1]);
	close(p[0]);

	// Attend la fin des processus
	wait(&statutExec1);
	wait(&statutExec2);

	// Vérifie si les processus se sont terminés normalement
	if(WIFEXITED(statutExec1) && WIFEXITED(statutExec2) && WEXITSTATUS(statutExec1) == 0 && WEXITSTATUS(statutExec2) == 0 )
		write(1, "root est connecté\n", 19);
	return 0;
    
}

