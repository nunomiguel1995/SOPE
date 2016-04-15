/* REMOVE FICHEIROS DUPLICADOS E SUBSTITUI POR HARDLINK PARA O MAIS ANTIGO */
/* USO: rmdup dirname */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]){

	if(argc != 2){
		printf("Usage: %s <dir>\n", argv[0]);
		return 1;
	}

	char *buf;
	char *listdir;
	char *dir;
	long size;

	size = pathconf(".", _PC_PATH_MAX);

	if((buf = (char *)malloc((size_t) size)) != NULL){
		listdir= getcwd(buf, (size_t)size);
	}

	if(listdir == NULL){
		printf("Error getting current working path\n");
		return 1;
	}

	dir= getenv("HOME");

	strcat(listdir,"/");
	strcat(listdir,"listdir");
	strcat(dir,"/");
	strcat(dir,argv[1]);

	execlp(listdir,"listdir",dir,NULL);

	return 0;
}
