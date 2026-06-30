#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_sig(int sig) {
    if (sig == SIGINT) {
        printf("\nCaught SIGINT (Interactive Interrupt). Cleaning up...\n");
    } else if (sig == SIGTERM) {
        printf("\nCaught SIGTERM (Polite Termination). Cleaning up...\n");
    }
    printf("Graceful shutdown complete.\n");
    exit(0);
}

int main() {
    signal(SIGINT, handle_sig);
    signal(SIGTERM, handle_sig);

    printf("Signal demo running. PID: %d\n", getpid());
    printf("Send an interrupt by pressing Ctrl+C...\n");

    while(1) {
        printf("OrbitWorks backend running...\n");
        sleep(2);
    }
    return 0;
}
