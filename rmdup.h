#ifndef RMDUP_H
#define RMDUP_H

/*Struct File
 * Save information of a regular file
 */
typedef struct File{
	char name[BUF_LENGTH];
	char date[BUF_LENGTH];
	char permissions[BUF_LENGTH];
	char path[BUF_LENGTH];
}File;

/* Checks if the content of two files is the same
 * @ret number of different lines
 */
int same_content(char path1[], char path2[]);

/* Creates a hardlink on the more recente duplicate files
 * @ret non zero on failure
 */
int link_Files(File* files, int array_size);

/* Reads files.txt and saves all the information on a array of struct File
 * @ret non zero on failure
 */
int read_Files();

int main(int argc, char *argv[], char *envp[]);

#endif
