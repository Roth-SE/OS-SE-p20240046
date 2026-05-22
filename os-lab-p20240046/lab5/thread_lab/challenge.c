#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

// Global flag to control thread execution
// volatile sig_atomic_t ensures safe read/writes between the signal handler and threads
volatile sig_atomic_t keep_running = 1;

// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int sig) {
    printf("\n[Signal Handler] Caught SIGINT (Ctrl+C). Initiating graceful shutdown...\n");
    keep_running = 0; // Tell threads to stop
}

// Worker thread function
void* worker_function(void* arg) {
    int thread_num = *((int*)arg);
    pthread_t tid = pthread_self();
    
    // Loop continues as long as keep_running is true (1)
    while (keep_running) {
        printf("Thread %d (ID: %lu) is working...\n", thread_num, (unsigned long)tid);
        sleep(1);
    }
    
    printf("Thread %d (ID: %lu) saw keep_running=0. Exiting.\n", thread_num, (unsigned long)tid);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[2];
    int thread_args[2] = {1, 2};

    // 1. Set up the signal handler
    signal(SIGINT, handle_sigint);
    printf("Main thread started. Press Ctrl+C to stop.\n");

    // 2. Spawn two worker threads
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&threads[i], NULL, worker_function, &thread_args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // 3. Wait for both threads to finish (this blocks until threads exit)
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    // 4. Print final exit message
    printf("All threads cleanly exited. Goodbye.\n");

    return 0;
}