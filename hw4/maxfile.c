#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>


void maxfile(const char *mydir,
            char **maxwrit, off_t *maxwritsize,
            char **maxnonwrit, off_t *maxnonwritsize,
            uint64_t *total_size) {
    
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    //open dir with error check
    dir = opendir(mydir);
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory.");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        //set filepath to a char array for function params
        char file_path[PATH_MAX];
        snprintf(file_path, sizeof(file_path), "%s/%s", mydir, entry->d_name);

        if (stat(file_path, &file_stat) == 0) {

            //if not a dir process
            if (S_ISREG(file_stat.st_mode)) {

                //check if file is writable and set values
                int is_writable = access(file_path, W_OK) == 0;
                if (is_writable && file_stat.st_size > *maxwritsize) {
                    if (*maxwrit) {
                        free(*maxwrit);
                    }
                    *maxwritsize = file_stat.st_size;
                    *maxwrit = strdup(file_path);
                } else if (!is_writable && file_stat.st_size > *maxnonwritsize) {
                    if (*maxnonwrit) {
                        free(*maxnonwrit);
                    }
                    *maxnonwritsize = file_stat.st_size;
                    *maxnonwrit = strdup(file_path);
                }

                //add file size to sum
                *total_size += file_stat.st_size;
            //if is a dir and not . or .. make a recursive call
            } else if (S_ISDIR(file_stat.st_mode) && (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)) {
                maxfile(file_path,
                    maxwrit, maxwritsize,
                    maxnonwrit, maxnonwritsize,
                    total_size);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./maxfile <directory>\n");
        exit(EXIT_FAILURE);
    }

    char *maxwrit, *maxnonwrit;
    off_t maxwritsize, maxnonwritsize;
    uint64_t total_size = 0;

    maxfile(argv[1],
        &maxwrit, &maxwritsize,
        &maxnonwrit, &maxnonwritsize,
        &total_size);

    printf("Largest writable filename: %s \n", maxwrit);
    printf("Largest writable file size: %ld bytes \n", maxwritsize);
    printf("Largest non-writable filename: %s \n", maxnonwrit);
    printf("Largest non-writable file size: %ld bytes \n", maxnonwritsize);
    printf("Total disk usage: %lu bytes\n", total_size);

    free(maxwrit);
    free(maxnonwrit);

    return 0;
}