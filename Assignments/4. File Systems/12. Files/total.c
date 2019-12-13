#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

unsigned long count(char *path) {

  unsigned long total = 0;
  
  DIR *dirp = opendir(path);

  if(dirp == NULL) {
    printf("not able to open %s\n", path);
    return 0;
  }
  
  char subdir[1024];  
  
  struct dirent *entry;
  
  while((entry = readdir(dirp)) != NULL) {
    switch( entry->d_type) {

    case DT_DIR:  //This is a directory.
      if((strcmp(entry->d_name, ".") == 0)  | (strcmp(entry->d_name, "..") == 0)) {
	break;
      };

      sprintf(subdir, "%s/%s", path, entry->d_name);
      total += count(subdir);
      break;

    case DT_REG:  //This is a regular file.
      // printf("f: %s\n", entry->d_name);
      total++;
      break;

    default:
      break;
    }

  }
  closedir(dirp);  
  return total;
}  



int main(int argc, char *argv[]) {

  if( argc < 2 ) {
    perror("usage: freq <dir>\n");
    return -1;
  }

  char *path = argv[1];

  unsigned long total =  count(path);

  printf("The directory %s contains %lu files\n", path, total);
    
}

