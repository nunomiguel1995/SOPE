/* REMOVE FICHEIROS DUPLICADOS E SUBSTITUI POR HARDLINK PARA O MAIS ANTIGO */
/* USO: rmdup dirname */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"

typedef struct File{
	char* name;
	char* date;
	char* permissions;
	char* path;
}File;

int sameContent(File file1, File file2){
	int cont1, cont2;

	FILE *f1, *f2;

	f1 = fopen(file1.path,"r");
	f2 = fopen(file2.path,"r");

	if(f1 == NULL){
		perror("file ERROR");
		fclose(f1);
		fclose(f2);
		return -1;
	}else if(f2 == NULL){
		perror("file ERROR");
		fclose(f1);
		fclose(f2);
		return -1;
	}else{
		cont1 = getc(f1);
		cont2 = getc(f2);

		while((cont1 != EOF) && (cont2 != EOF) && (cont1 == cont2)){
			cont1 = getc(f1);
			cont2 = getc(f2);
		}

		if(cont1 == cont2){ /*conteúdo igual*/
			fclose(f1);
			fclose(f2);
			return 0;
		}else{ /*conteúdo diferente*/
			fclose(f1);
			fclose(f2);
			return 1;
		}
	}

	fclose(f1);
	fclose(f2);
	return 0;
}

int link_Files(File* files, int array_size){
	int i, j;

	for(i = 0; i < array_size; i++){
		j = i + 1;
		while(strcmp(files[i].name,files[j].name) == 0){
			unlink(files[j].name);
			link(files[i].path,files[j].path);
			j++;
		}
		i += j;
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
		files[i].name = malloc(sizeof(char*));
		files[i].date = malloc(sizeof(char*));
		files[i].permissions = malloc(sizeof(char*));
		files[i].path = malloc(sizeof(char*));

		fscanf(f,"%s %s %s %s", files[i].name,files[i].date, files[i].permissions, files[i].path);
	}
	fclose(f);

	link_Files(files,array_size);

	return 0;
}

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
		perror("working path ERROR");
		return 1;
	}

	dir= getenv("HOME");

	strcat(listdir,"/");
	strcat(listdir,"listdir");
	strcat(dir,"/");
	strcat(dir,argv[1]);

	execlp(listdir,"listdir",dir,NULL);

	if(read_Files() == -1){
		perror("reading file data ERROR");
		return 1;
	}

	return 0;
}
