#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

unsigned long  count(char *path) {

	unsigned long total = 0;

	DIR * dirp = opendir(path);

	char subdir[1024];

	struct dirent *entry;

	struct stat file_st;

	while((entry = readdir(dirp)) != NULL) {
		switch(entry -> d_type) {
			case DT_BLK :		// This is a block device
				printf("b:");
				break;
			case DT_CHR :		// This is a character device
				printf("c:");
				break;
			case DT_DIR :		// This is a directory
				printf("d:");
				sprintf(subdir, "%s/%s", path, entry -> d_name);
				total += count(subdir);
				break;
			case DT_FIFO :		// This is a named pipe
				printf("p:");
				break;
			case DT_LNK :		// This is a symbolic link
				printf("l:");
				break;
			case DT_REG :		// This is a regular file
				printf("f:");
				total++;
				break;
			case DT_SOCK : 		// This is a UNIX domain socket
				printf("s:");
				break;
			case DT_UNKNOWN :	// The file type is unknown
				printf("u:");
				break;
			default :
				break;
		}
	if((strcmp(entry->d_name, ".") == 0) | (strcmp(entry->d_name, "..") == 0)) {
    break;
	}

	closedir(dirp);
	return total;
}
}


int main(int argc, char *argv[]) {
	if(argc <2) {
		perror("usage: total <dir>\n");
		return -1;
	}

	char *path = argv[1];
	unsigned long total = count(path);
	printf("The directory %s contains %lu files\n", path, total);
}