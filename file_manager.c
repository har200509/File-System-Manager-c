#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define MAX_PATH 1024

// Function prototypes
void createFile(const char *filename);
void readFile(const char *filename);
void writeFile(const char *filename, const char *content);
void deleteFile(const char *filename);
void setFilePermissions(const char *filename, mode_t mode);
void listFiles(const char *directory);
void displayFileAttributes(const char *filename);
void renameFile(const char *oldname, const char *newname);
void moveFile(const char *filename, const char *newpath);
void copyFile(const char *source, const char *destination);
void sortFiles(const char directory, int (*compare)(const void, const void*));
int compareSize(const void *a, const void *b);
int compareTime(const void *a, const void *b);

typedef struct {
    char name[256];
    off_t size;
    time_t mtime;
} FileInfo;

void createFile(const char *filename) {
    int fd = open(filename, O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("Error creating file");
        return;
    }
    printf("File '%s' created successfully.\n", filename);
    close(fd);
}

void readFile(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }
    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytesRead);
    }
    if (bytesRead == -1) {
        perror("Error reading file");
    }
    close(fd);
}

void writeFile(const char *filename, const char *content) {
    int fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }
    if (write(fd, content, strlen(content)) == -1) {
        perror("Error writing to file");
    }
    close(fd);
}

void deleteFile(const char *filename) {
    if (unlink(filename) == 0) {
        printf("File '%s' deleted successfully.\n", filename);
    } else {
        perror("Error deleting file");
    }
}

void setFilePermissions(const char *filename, mode_t mode) {
    if (chmod(filename, mode) == 0) {
        printf("Permissions for '%s' set successfully.\n", filename);
    } else {
        perror("Error setting file permissions");
    }
}

void listFiles(const char *directory) {
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

void displayFileAttributes(const char *filename) {
    struct stat fileStat;
    if (stat(filename, &fileStat) < 0) {
        perror("Error getting file attributes");
        return;
    }

    printf("File: %s\n", filename);
    printf("Size: %ld bytes\n", fileStat.st_size);
    printf("Last modified: %s", ctime(&fileStat.st_mtime));
}

void renameFile(const char *oldname, const char *newname) {
    if (rename(oldname, newname) == 0) {
        printf("File renamed successfully from '%s' to '%s'.\n", oldname, newname);
    } else {
        perror("Error renaming file");
    }
}

void moveFile(const char *filename, const char *newpath) {
    char newfilepath[MAX_PATH];
    snprintf(newfilepath, sizeof(newfilepath), "%s/%s", newpath, filename);
    if (rename(filename, newfilepath) == 0) {
        printf("File '%s' moved to '%s' successfully.\n", filename, newpath);
    } else {
        perror("Error moving file");
    }
}

void copyFile(const char *source, const char *destination) {
    int sourcefd, destfd;
    char buffer[4096];
    ssize_t nread;

    sourcefd = open(source, O_RDONLY);
    if (sourcefd == -1) {
        perror("Error opening source file");
        return;
    }

    destfd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destfd == -1) {
        perror("Error creating destination file");
        close(sourcefd);
        return;
    }

    while ((nread = read(sourcefd, buffer, sizeof(buffer))) > 0) {
        if (write(destfd, buffer, nread) != nread) {
            perror("Error writing to destination file");
            close(sourcefd);
            close(destfd);
            return;
        }
    }

    close(sourcefd);
    close(destfd);

    if (nread == -1) {
        perror("Error reading from source file");
    } else {
        printf("File copied successfully from '%s' to '%s'.\n", source, destination);
    }
}

int compareSize(const void *a, const void *b) {
    return ((FileInfo*)a)->size - ((FileInfo*)b)->size;
}

int compareTime(const void *a, const void *b) {
    return ((FileInfo*)b)->mtime - ((FileInfo*)a)->mtime;
}

void sortFiles(const char directory, int (*compare)(const void, const void*)) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    FileInfo *files = NULL;
    int count = 0;
    char filepath[MAX_PATH];

    dir = opendir(directory);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            snprintf(filepath, sizeof(filepath), "%s/%s", directory, entry->d_name);
            if (stat(filepath, &st) == 0) {
                files = realloc(files, (count + 1) * sizeof(FileInfo));
                if (files == NULL) {
                    perror("Memory allocation error");
                    closedir(dir);
                    return;
                }
                strncpy(files[count].name, entry->d_name, sizeof(files[count].name) - 1);
                files[count].name[sizeof(files[count].name) - 1] = '\0';
                files[count].size = st.st_size;
                files[count].mtime = st.st_mtime;
                count++;
            }
        }
    }
    closedir(dir);

    if (count > 0) {
        qsort(files, count, sizeof(FileInfo), compare);

        printf("Sorted files:\n");
        for (int i = 0; i < count; i++) {
            printf("%s (Size: %ld bytes, Modified: %s)", files[i].name, files[i].size, ctime(&files[i].mtime));
        }

        free(files);
    } else {
        printf("No files found in the directory.\n");
    }
}

int main() {
    // Example usage of all functions
    createFile("file1.txt");
    createFile("file2.txt");
    writeFile("file1.txt", "Hello, World!\n");
    writeFile("file2.txt", "Another file content.\n");
    
    printf("Contents of file1.txt:\n");
    readFile("file1.txt");
    printf("\nContents of file2.txt:\n");
    readFile("file2.txt");
    
    setFilePermissions("file1.txt", 0644);
    
    printf("\nListing files in current directory:\n");
    listFiles(".");
    
    printf("\nFile attributes for file1.txt:\n");
    displayFileAttributes("file1.txt");
    
    renameFile("file2.txt", "renamed_file2.txt");
    copyFile("file1.txt", "file1_copy.txt");
    
    // Create a directory for moving files
    if (mkdir("moved", 0755) == -1 && errno != EEXIST) {
        perror("Error creating directory");
    } else {
        moveFile("file1.txt", "moved");
    }
    
    printf("\nSorting files by size:\n");
    sortFiles(".", compareSize);
    
    printf("\nSorting files by last modification time:\n");
    sortFiles(".", compareTime);
    
    // Clean up
    deleteFile("renamed_file2.txt");
    deleteFile("file1_copy.txt");
    deleteFile("moved/file1.txt");
    rmdir("moved");
    
    return 0;
}