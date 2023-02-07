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

int main(int argc, char const *argv[])
{
    struct stat s;
	int err = stat(argv[1], &s);
	if(-1 == err) {
		if(ENOENT == errno) {
			dprintf(stderr, "Error : The path does not match to a dir or file\n");
			perror("stat");
			// Exiting with a failure ERROR CODE (== 1)
			exit(EXIT_FAILURE);
		} 
		else {
			perror("stat");
			exit(1);
		}
	} 
	else {
		if(S_ISDIR(s.st_mode)) {
			DIR* dir = opendir(argv[1]) ;
			struct dirent *dirStruct ;
			dprintf(stdout,"Début dossier\n");
			while(dirStruct=readdir(dir))
			{
				stat((dirStruct->d_name), &s);
				printf("%-30s",(dirStruct->d_name));
				printf("  ");         
				printf( (S_ISDIR(s.st_mode)) ? "d" : "-");
				printf( (s.st_mode & S_IRUSR) ? "r" : "-");
				printf( (s.st_mode & S_IWUSR) ? "w" : "-");
				printf( (s.st_mode & S_IXUSR) ? "x" : "-");
				printf( (s.st_mode & S_IRGRP) ? "r" : "-");
				printf( (s.st_mode & S_IWGRP) ? "w" : "-");
				printf( (s.st_mode & S_IXGRP) ? "x" : "-");
				printf( (s.st_mode & S_IROTH) ? "r" : "-");
				printf( (s.st_mode & S_IWOTH) ? "w" : "-");
				printf( (s.st_mode & S_IXOTH) ? "x" : "-");
				printf("  ");         
				struct passwd *pwd;
				pwd = getpwuid(s.st_uid);
				printf("%s",getpwuid(pwd.pw_name));
				printf("  ");         
				printf("%d",s.st_uid);
				printf(" : ");         
				printf("%d",s.st_gid);
				printf("  ");         

				time_t rawtime;
				struct tm * timeinfo;
				char buffer[80];
				time ( &s.st_mtime );
				timeinfo = localtime ( &s.st_mtime );
				strftime(buffer,80,"%x - %I:%M%p", timeinfo);
				printf("|%s| ", buffer );



				printf("%d bytes\n",s.st_size);
			}
			dprintf(stdout,"Fin dossier\n");

		} 
		else {
			dprintf(stderr, "Error : The path does not match to a dir but a file\n");
			perror("stat");
			// Exiting with a failure ERROR CODE (== 1)
			exit(EXIT_FAILURE);
		}
	}

	return 1;
}

