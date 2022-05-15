#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

long min;
long max;
long state;

void GoThroughFiles(char *dirName, FILE *file) 
{
    DIR *currDir;
    struct dirent *dp;
    struct stat buf;
   
    char *dirDown;

    if ((currDir = opendir(dirName)) == NULL) 
    {
        perror("Couldn't open the directory");
        return;
    }

    while ((dp = readdir(currDir)) != NULL) 
    {
        if (dp->d_type == DT_DIR && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) 
        {
            dirDown = (char*) calloc(strlen(dirName)+1+ strlen(dp->d_name), sizeof(char));
            strcpy(dirDown, dirName);
            strcat(dirDown, "/");
            strcat(dirDown, dp->d_name);
            GoThroughFiles(dirDown, file);
        }

        else if (dp->d_type == DT_REG)
        {
            char *filePath = (char*) calloc(strlen(dirName) + 1 + strlen(dp->d_name), sizeof(char));
            strcpy(filePath, dirName);
            strcat(filePath, "/");
            strcat(filePath, dp->d_name);

            if (stat(filePath, &buf) == 0) 
            {
                if (state == 0 && min <= buf.st_size && buf.st_size <= max)
                {
                    fprintf(file, "%s:     %ld %ld\n", filePath, buf.st_size, (long)buf.st_ctim.tv_sec);
                }
                else if (state == 1 && min <= buf.st_ctim.tv_sec && buf.st_ctim.tv_sec <= max) 
                {
                    fprintf(file, "%s:     %ld %ld\n", filePath, buf.st_size, (long)buf.st_ctim.tv_sec);
                }
            }
        }
    }

    if (closedir(currDir) == -1) 
    {
        perror("Couldn't close directory");
    }

    return;
}

long convertStrToNum(char *str) 
{
    char *ptr;
    long res = strtol(str, &ptr, 10);
    errno = 0;
    if (errno == ERANGE && (errno != 0 && res == 0)) 
    {
        perror("strol");
        exit(-1);
    }
    if (str == ptr) 
    {
        fprintf(stderr, "No digits");
        exit(-1);
    }

    return res;
}

int main(int argc, char *argv[]) 
{
    if (argc != 6) 
    {
        fprintf(stderr ,"1 argument - name of directory\n2 argument - output file\n3, 4 - min and max sizes\n5 - state (1 - time; \n0 - size)");
        return -1;
    }

    min = convertStrToNum(argv[3]);
    max = convertStrToNum(argv[4]);
    state = convertStrToNum(argv[5]);

    if (min > max)
    {
        fprintf (stderr, "Min must be less, then max");
        return -1;
    }

    if (state != 0 && state != 1)
    {
        fprintf(stderr, "Incorrect value of state");
        return -1;
    }

    int fileHandler = open(argv[2], O_TRUNC | O_WRONLY | O_CREAT, 0666);
    if (fileHandler < 0) 
    {
        perror("open");
        return -1;
    }

    errno = 0;
    FILE *file = fdopen(fileHandler, "w");
    if (errno != 0) 
    {
        perror("fdopen");
        return -1;
    }

    GoThroughFiles(argv[1], file);
    return 0;
}