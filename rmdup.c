#include "utilities.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"
#include "rmdup.h"


int sameContent(char path1[], char path2[]){
	int pid, status, res;

	pid = fork();
	if(pid > 0){ /*pai*/
		waitpid(pid,&status,0);
		if(status == 0){
			res = 0; //iguais
		}else{
			res = 1;
		}
	}else{ /*filho*/
		execlp("diff","diff", path1, path2, NULL);
	}
	return res;
}

int link_Files(File* files, int array_size){
	int i = 0, j = 0, saveFile;
	char linkFile[BUF_LENGTH];


	saveFile = open("linkFiles.txt",O_WRONLY|O_CREAT|O_TRUNC, 0664);
	if(saveFile == -1){
		perror("file ERROR");
		close(saveFile);
		exit(1);
	}

	for(i = 0; i < array_size; i++){
		j = i + 1;
		while(strcmp(files[i].name,files[j].name) == 0){
			if(strcmp(files[i].permissions,files[j].permissions) == 0){
				if(sameContent(files[i].path,files[j].path) == 0){
					if(unlink(files[j].path) == -1){
						perror("unlink ERROR");
						return -1;
					}
					if(link(files[i].path,files[j].path) == -1){
						perror("unlink ERROR");
						return -1;
					}
					sprintf(linkFile,"%s %s %s %s\n",files[j].name,files[j].permissions, files[j].date , files[j].path);
					write(saveFile,linkFile,strlen(linkFile));
				}
			}
			j++;
		}
		i = j - 1;
	}

	return 0;
}

int read_Files(){ /*inicializa vetor com dados de ficheiros*/
	int c = 0;
	int nLines = 1;
	int i;
	int array_size;
	FILE* f = fopen(FILE_NAME,"r");

	if(f == NULL){
		perror("file ERROR");
		return -1;
	}
	while((c = fgetc(f)) != EOF){
		if(c == '\n') nLines++;
	}
	fclose(f);

	f = fopen(FILE_NAME,"r");
	array_size = nLines - 1;

	File* files = malloc(array_size * sizeof(*files));
	for(i = 0; i < array_size; i++){
		fscanf(f,"%s %s %s %s", files[i].name,files[i].date, files[i].permissions, files[i].path);
	}
	fclose(f);

	if(link_Files(files,array_size) == -1){
		perror("linking files ERROR");
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[], char *envp[]){
	int pid;

	if(argc != 2){
		printf("Usage: %s <dir>\n", argv[0]);
		return 1;
	}

	char *buf;
	char *listdir;
	char *path=NULL;
	long size;


	size = pathconf(".", _PC_PATH_MAX);
	if((buf = (char *)malloc((size_t) size)) != NULL){
		listdir= getcwd(buf, (size_t)size);
	}
	if(listdir == NULL){
		perror("working path ERROR");
		return 1;
	}
	strcat(listdir,"/");
	strcat(listdir,"listdir");

	/*Checks if user passes a absolute or relative path*/
	char *c= argv[1];

	if(c[0]=='/'){ //Absolute path
		path= argv[1];
	}
	else if(c[0]!=0){//Relative path
		long path_size;
		char *buffer;

		path_size = pathconf(".", _PC_PATH_MAX);

		if((buffer = (char *)malloc((size_t) path_size)) != NULL){
			path= getcwd(buffer, (size_t)size);
		}

		if(path == NULL){
			perror("working path ERROR");
			return 1;
		}

		strcat(path,"/");
		strcat(path,argv[1]);
	}

	pid = fork();
	if(pid > 0){ /*pai*/
		waitpid(pid,NULL,0);
		if(read_Files() == -1){
			perror("reading file data ERROR");
			return 1;
		}
	}else{ /*filho*/
		execlp(listdir,"listdir",path,NULL);
	}

	return 0;
}

