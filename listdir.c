/* LISTAR FICHEIROS REGULARES E SUB-DIRECTÓRIOS DE UM DIRECTÓRIO */
/* USO: listdir dirname */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <fcntl.h>
#include "utilities.h"

int read_directory(char *dir_name, int file){
	DIR *dirp;
	struct dirent *direntp;
	struct stat stat_buf;
	char name[200];
	char f_name[50];
	int pid;

	if ((dirp = opendir( dir_name)) == NULL){
		return 1;
	}

	while ((direntp = readdir( dirp)) != NULL){
		if((strcmp(direntp->d_name,".") != 0) && (strcmp(direntp->d_name,"..") != 0)){ /*nao lista ficheiros ocultos*/
			sprintf(name,"%s/%s",dir_name,direntp->d_name);
			if (lstat(name, &stat_buf)==-1)	{
				return 2;
			}
			if (S_ISREG(stat_buf.st_mode)){
				strcpy(f_name,direntp->d_name);
				strcat(f_name,"\n");
				write(file,f_name,strlen(f_name));
			}else if (S_ISDIR(stat_buf.st_mode)){
				pid = fork();
				if(pid > 0){ /*pai*/
					waitpid(pid,NULL,0);
				}else{ /*filho*/
					read_directory(name,file);
					return 0;
				}
			}
		}
	}

	closedir(dirp);
	return 0;
}

int main(int argc, char *argv[]){
	int file;
	int read_dir;

	if (argc != 2){
		fprintf(stderr, "Usage: %s dir_name\n", argv[0]);
		exit(1);
	}

	open(FILE_NAME, O_TRUNC);
	file = open(FILE_NAME,O_WRONLY | O_APPEND | O_CREAT, S_IRWXG | S_IRWXU | S_IROTH);

	if(file == -1){
		perror("file ERROR");
		exit(1);
	}

	read_dir = read_directory(argv[1],file);

	if(read_dir == 1){
		perror(argv[1]);
		exit(2);
	}else if(read_dir == 2){
		perror("lstat ERROR");
		exit(3);
	}

	close(file);

	exit(0);
}
