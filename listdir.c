/* LISTAR FICHEIROS REGULARES E SUB-DIRECTÓRIOS DE UM DIRECTÓRIO */
/* USO: listdir dirname */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

int read_directory(char *dir_name){

	DIR *dirp;
	struct dirent *direntp;
	struct stat stat_buf;
	char *str;
	char name[200];

	if ((dirp = opendir( dir_name)) == NULL){
		return 1;
	}

	while ((direntp = readdir( dirp)) != NULL){
		if(direntp->d_name[0] != '.'){ //nao lista ficheiros ocultos
			sprintf(name,"%s/%s",dir_name,direntp->d_name);
			if (lstat(name, &stat_buf)==-1)	{
				return 2;
			}
			if (S_ISREG(stat_buf.st_mode)) str = "regular";
			else if (S_ISDIR(stat_buf.st_mode)) str = "directory";
			else str = "other";
			printf("%-25s - %s\n", direntp->d_name, str);
		}
	}
	closedir(dirp);
	return 0;
}

int main(int argc, char *argv[]){
	if (argc != 2){
		fprintf( stderr, "Usage: %s dir_name\n", argv[0]);
		exit(1);
	}

	int read_dir = read_directory(argv[1]);

	if(read_dir == 1){
		perror(argv[1]);
		exit(2);
	}else if(read_dir == 2){
		perror("lstat ERROR");
		exit(3);
	}

	exit(0);
}
