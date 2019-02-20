#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t getFileSize(const char* filename) {
    struct stat st;
    if(lstat(filename, &st) != 0) {
        return 0;
    }
    return st.st_size;
}

int isRegularFile(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
int total = 0;

int main(int argc, char *argv[]) {
    if (argv[1] == NULL) {
        argv[1] = "/";
        printf("Path not entered, will be used default: %s", argv[1]);
    }

    for ( int i = 1; i < argc; ++i ) {
        printf("%s\n", argv[i]);
        if (isRegularFile(argv[i]) < 1) {
            DIR *dr = opendir(argv[i]);
            if(dr == NULL) {
                printf("Could not open current directory\n");
            } else {
                struct stat buf;
                struct dirent *sdir;
                int size=0;
                char str[1024];

                while((sdir = readdir(dr))) {
                    if(sdir->d_type == 4) {
                        if(sdir->d_name[0]!='.') {
                            stat(sdir->d_name, &buf);
                            size = buf.st_size;
                            total += size;
                        }
                    } else {
                        strcpy(str, argv[i]);
                        strcat(str, "/");
                        strcat(str, sdir->d_name);
                        stat(str, &buf);
                        size += buf.st_size;
                        total += size;
                    }
                }

                printf("Folder size: %d kb\n", total / 1024);
            }
        } else {
            float size = getFileSize(argv[i]);

            printf("Size of file %zu kb\n", size);
        }
        total = 0;
        printf("----------------------------------\n");
    }

    exit(EXIT_SUCCESS);
}