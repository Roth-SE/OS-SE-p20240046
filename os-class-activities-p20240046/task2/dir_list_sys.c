/* dir_list_sys.c */
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h> 
int main() {
    char buffer[512];
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    int len;

    // Open current directory with opendir(".")
    dir = opendir(".");
    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }
    // Print header line using write()
    // We format the headers into the buffer, then write the buffer to fd 1
    len = snprintf(buffer, sizeof(buffer), "%-30s %10s\n", "Filename", "Size (bytes)");
    write(1, buffer, len);
    len = snprintf(buffer, sizeof(buffer), "%-30s %10s\n", "------------------------------", "----------");
    write(1, buffer, len);
    // Loop through entries with readdir()
    while ((entry = readdir(dir)) != NULL) {
        // For each entry, use stat() to get file size
        if (stat(entry->d_name, &fileStat) == 0) {
            // Format output into buffer with snprintf(), then write() to fd 1
            len = snprintf(buffer, sizeof(buffer), "%-30s %10ld\n", entry->d_name, fileStat.st_size);
            write(1, buffer, len);
        }
    }
    closedir(dir);

    return 0;
}