

typedef struct File{
	char name[BUF_LENGTH];
	char date[BUF_LENGTH];
	char permissions[BUF_LENGTH];
	char path[BUF_LENGTH];
}File;

int read_directory(char *dir_name, int file);

int sameContent(char path1[], char path2[]);

int link_Files(File* files, int array_size);

int read_Files();

int main(int argc, char *argv[], char *envp[]);

