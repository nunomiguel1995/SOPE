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
#include <time.h>

#include "lsdir.h"
#include "utilities.h"

int read_directory(char *dir_name, int file){
	DIR *dirp;
	struct dirent *direntp;
	struct stat stat_buf;
	char path[200];
	char f_name[50];
	int pid;
	int p_mask;
	int mod_time;

	if ((dirp = opendir( dir_name)) == NULL){
		return 1;
	}

	while ((direntp = readdir( dirp)) != NULL){
		if((strcmp(direntp->d_name,".") != 0) && (strcmp(direntp->d_name,"..") != 0)){ /*nao lista ficheiros ocultos*/
			sprintf(path,"%s/%s",dir_name,direntp->d_name);
			if (lstat(path, &stat_buf)==-1)	{
				return 2;
			}
			if (S_ISREG(stat_buf.st_mode)){
				p_mask = (stat_buf.st_mode & 0777); //máscara de permissões
				mod_time = stat_buf.st_mtim.tv_sec; //data da última modificação

				sprintf(f_name,"%s %3o %d %s\n",direntp->d_name, p_mask, mod_time , path); //formata a linha a guardar no ficheiro
				write(file,f_name,strlen(f_name));
			}else if (S_ISDIR(stat_buf.st_mode)){
				pid = fork();
				if(pid > 0){ /*pai*/
					waitpid(pid,NULL,0);
				}else{ /*filho*/
					read_directory(path,file);
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

	file = open(FILE_NAME,O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0664);

	if(file == -1){
		perror("file ERROR");
		close(file);
		exit(1);
	}

	read_dir = read_directory(argv[1],file);

	if(read_dir == 1){
		perror(argv[1]);
		close(file);
		exit(2);
	}else if(read_dir == 2){
		perror("lstat ERROR");
		close(file);
		exit(3);
	}

	close(file);
	execlp("sort", "sort", FILE_NAME,"-no", FILE_NAME, (char*)NULL);

	exit(0);
}
