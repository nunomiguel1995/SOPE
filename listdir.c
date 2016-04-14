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

int read_directory(char *dir_name, FILE *file){
	DIR *dirp;
	struct dirent *direntp;
	struct stat stat_buf;
	char name[200];
	char *path = strcat(dir_name,"/");
	pid_t pid;

	if ((dirp = opendir( dir_name)) == NULL){
		return 1;
	}

	while ((direntp = readdir( dirp)) != NULL){
		if(direntp->d_name[0] != '.'){ /*nao lista ficheiros ocultos*/
			sprintf(name,"%s/%s",dir_name,direntp->d_name);
			if (lstat(name, &stat_buf)==-1)	{
				return 2;
			}
			if (S_ISREG(stat_buf.st_mode)){
				char f_name[50];
				strcpy(f_name,direntp->d_name);
				fprintf(file,"%s\n",f_name);
			}
			else if (S_ISDIR(stat_buf.st_mode)){
				pid = fork();
				if(pid == 0){ /*filho*/
					read_directory(strcat(path,direntp->d_name),file);
					return 0;
				}else{ /*pai*/
					waitpid(pid,NULL,0);
				}
			}
		}
	}

	closedir(dirp);
	return 0;
}

int main(int argc, char *argv[]){
	FILE *file;
	int read_dir;

	if (argc != 2){
		fprintf(stderr, "Usage: %s dir_name\n", argv[0]);
		exit(1);
	}

	file = fopen(FILE_NAME,"w");
	if(file == NULL){
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

	fclose(file);
	exit(0);
}
