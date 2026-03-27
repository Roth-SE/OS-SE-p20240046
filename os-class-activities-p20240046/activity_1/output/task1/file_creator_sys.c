/* file_creator_sys.c */
#include <fcntl.h>    // open(), O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h>   // write(), close()
#include <string.h>   // strlen()

int main() {
    const char *fileMessage = "Hello from Operating Systems class!\n";
    const char *okMessage = "File created successfully!\n";
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0) {
        return 1;
    }

    write(fd, fileMessage, strlen(fileMessage));
    close(fd);
    write(1, okMessage, strlen(okMessage));
    return 0;
}