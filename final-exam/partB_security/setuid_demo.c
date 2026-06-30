#include <stdio.h>
#include <unistd.h>

int main() {
    printf("OrbitWorks Security Check\n");
    printf("Real UID: %d\n", getuid());
    printf("Effective UID: %d\n", geteuid());
    return 0;
}
