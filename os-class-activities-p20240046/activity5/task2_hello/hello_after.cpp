/*
 * Task 2B: Print HELLO - AFTER Semaphores (Correct Order)
 *
 * Compile:  g++ -Wall -Wextra -g3 -pthread -o hello_after hello_after.cpp
 * Run:      ./hello_after
 *
 * Uses POSIX semaphores (sem_t) — works with C++11/14/17 on any Linux/WSL.
 *
 * Semaphore chain forces H → E → L → L → O regardless of thread scheduling:
 *
 *   start_h  (1) → Process1 prints H, E → signals after_e
 *   after_e  (0) → Process2 prints L   → signals after_l1
 *   after_l1 (0) → Process2 prints L   → signals after_l2
 *   after_l2 (0) → Process3 prints O
 */

#include <iostream>
#include <thread>
#include <semaphore.h>   // POSIX semaphores — no C++20 needed

// ── Semaphores (global, re-initialised each run) ──────────────────────────────
sem_t start_h;    // Process 1 may begin immediately  (init: 1)
sem_t after_e;    // unlocked after E is printed       (init: 0)
sem_t after_l1;   // unlocked after first L            (init: 0)
sem_t after_l2;   // unlocked after second L           (init: 0)

// ── Process 1: prints H then E ────────────────────────────────────────────────
void process1() {
    sem_wait(&start_h);                    // wait: permitted to begin
    std::cout << "H" << std::flush;
    std::cout << "E" << std::flush;
    sem_post(&after_e);                    // signal: E done, Process 2 may print L
}

// ── Process 2: prints first L, then second L ─────────────────────────────────
void process2() {
    sem_wait(&after_e);                    // wait: HE must be printed first
    std::cout << "L" << std::flush;
    sem_post(&after_l1);                   // signal: first L done

    sem_wait(&after_l1);                   // wait: re-acquire to print second L
    std::cout << "L" << std::flush;
    sem_post(&after_l2);                   // signal: second L done, Process 3 may go
}

// ── Process 3: prints O ───────────────────────────────────────────────────────
void process3() {
    sem_wait(&after_l2);                   // wait: LL must be printed first
    std::cout << "O" << std::flush;
}

// ── Main ──────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "=== AFTER SEMAPHORES: Printing HELLO in correct order ===\n";
    std::cout << "Running 5 times — output must always be HELLO:\n\n";

    for (int run = 1; run <= 5; ++run) {
        // Re-initialise semaphores for each run (pshared=0: thread-shared)
        sem_init(&start_h,  0, 1);
        sem_init(&after_e,  0, 0);
        sem_init(&after_l1, 0, 0);
        sem_init(&after_l2, 0, 0);

        std::cout << "Run " << run << ": " << std::flush;

        std::thread t1(process1);
        std::thread t2(process2);
        std::thread t3(process3);

        // Start all threads — semaphores enforce ordering regardless of schedule
        t1.join();
        t2.join();
        t3.join();

        std::cout << "\n";

        // Clean up semaphores for this run
        sem_destroy(&start_h);
        sem_destroy(&after_e);
        sem_destroy(&after_l1);
        sem_destroy(&after_l2);
    }

    std::cout << "\n[Done] HELLO printed correctly every time!\n";
    return 0;
}