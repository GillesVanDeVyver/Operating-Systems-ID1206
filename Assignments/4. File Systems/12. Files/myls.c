#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char *argv[]) {

    if(argc < 2) {
        perror("Usage: myls <dir>\n");
        return -1;
    }

    char *path = argv[1];

    DIR *dirp = opendir(path);

    struct dirent *entry;

    while ((entry = readdir(dirp)) != NULL) {
        printf("type: %u", entry->d_type);
        printf("\tinode %lu", entry->d_ino);
        printf("\tname: %s\n", entry->d_name);
    }
}
