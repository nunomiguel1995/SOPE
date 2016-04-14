/* REMOVE FICHEIROS DUPLICADOS E SUBSTITUI POR HARDLINK PARA O MAIS ANTIGO */
/* USO: rmdup dirname */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

  if(argc != 2){
    printf("Usage: %s <dir>\n", argv[0]);
    return 1;
  }

  execlp("/home/nuno/workspace/proj_sope/listdir","listdir",argv[1],NULL);

  return 0;
}
