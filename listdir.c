/* LISTAR FICHEIROS REGULARES DE UM DIRECTÓRIO */
/* USO: listdir dirname */

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]){
  DIR *dir;
  int line;
  struct dirent *dentry;
  struct stat stat_entry;

  if (argc != 2) {
    printf("Usage: %s <dir_path>\n", argv[0]);
    return 1;
  }
  if ((dir = opendir(argv[1])) == NULL) {
    perror(argv[1]);
    return 2;
  }

  chdir(argv[1]);
  printf("Ficheiros regulares do directorio '%s'\n", argv[1]);
  line = 1;

  while ((dentry = readdir(dir)) != NULL) {
    stat(dentry->d_name, &stat_entry);
    if (S_ISREG(stat_entry.st_mode)) {
        printf("%-25s%12d%3d\n", dentry->d_name, (int)stat_entry.st_size,(int)stat_entry.st_nlink);
        if (line++ % 20 == 0) {
          printf("Press <enter> to continue");
          getchar();
        }
    }
  }

  return 0;
}
