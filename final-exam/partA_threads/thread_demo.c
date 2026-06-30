#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5
#define EXTRA_THREADS 3

void* worker(void* arg) {
    long tid = (long)arg;
    int* result = malloc(sizeof(int));
    *result = (tid + 1) * 10; 
    printf("Worker thread %ld running, computed value: %d\n", tid, *result);
    sleep(8); // 8 seconds to capture the screenshot
    pthread_exit((void*)result);
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_t extra_threads[EXTRA_THREADS];
    int* return_val;
    int total_sum = 0;

    printf("Starting OrbitWorks thread demo (Original 5)...\n");
    for(long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, (void*)i);
    }
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], (void**)&return_val);
        total_sum += *return_val;
        free(return_val);
    }
    
    printf("Originals joined. Spawning 3 EXTRA workers...\n");
    for(long i = 0; i < EXTRA_THREADS; i++) {
        pthread_create(&extra_threads[i], NULL, worker, (void*)(i+NUM_THREADS));
    }
    for(int i = 0; i < EXTRA_THREADS; i++) {
        pthread_join(extra_threads[i], (void**)&return_val);
        total_sum += *return_val;
        free(return_val);
    }

    printf("All workers finished. Summary total: %d\n", total_sum);
    return 0;
}
