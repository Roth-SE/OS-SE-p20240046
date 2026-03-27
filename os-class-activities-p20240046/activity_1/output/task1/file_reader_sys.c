/* file_reader_sys.c */
#include <fcntl.h>
#include <unistd.h>

int main() {
    char buffer[256];
    int fd;
    ssize_t bytesRead;

    //Open "output.txt" for reading using open()
    fd = open("output.txt", O_RDONLY);
    if (fd == -1) {
        // Basic error handling if the file doesn't exist
        return 1;
    }

    //Read content into buffer using read() in a loop
    // read() returns the number of bytes read, or 0 at EOF
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        
        //Write the content to fd 1 using write()
        write(1, buffer, bytesRead);
    }
    close(fd);

    return 0;
}