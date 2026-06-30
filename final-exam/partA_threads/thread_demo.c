#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

void* worker(void* arg) {
    long tid = (long)arg;
    int* result = malloc(sizeof(int));
    
    // Compute a simple value
    *result = (tid + 1) * 10; 
    
    printf("Worker thread %ld running, computed value: %d\n", tid, *result);
    
    // A 15-second pause so you have time to capture the A2 thread map!
    sleep(15); 
    
    pthread_exit((void*)result);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int* return_val;
    int total_sum = 0;

    printf("Starting OrbitWorks thread demo...\n");

    // Spawn 5 worker threads
    for(long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, (void*)i);
    }

    // Join threads and collect results
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], (void**)&return_val);
        total_sum += *return_val;
        free(return_val);
    }

    printf("All workers finished. Summary total: %d\n", total_sum);
    return 0;
}
