/*
 * Task 1B: Particle Pair Buffer - AFTER Semaphores (Correct)
 *
 * Compile:  g++ -Wall -Wextra -g3 -pthread -o particles_after particles_after.cpp
 * Run:      ./particles_after        (press Ctrl+C to stop)
 *
 * Uses POSIX semaphores (sem_t) — works with C++11/14/17 on any Linux/WSL.
 *
 * Three semaphores:
 *   empty_pairs (50) – producers wait here; prevents buffer overflow
 *   full_pairs  ( 0) – consumer waits here; prevents buffer underflow
 *   mtx         ( 1) – mutex protecting all shared buffer/counter access
 */

#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <string>
#include <chrono>
#include <atomic>
#include <random>
#include <semaphore.h>   // POSIX semaphores — no C++20 needed

// ── Configuration ─────────────────────────────────────────────────────────────
static const int BUFFER_CAPACITY    = 100;  // 100 particles = 50 pairs
static const int NUM_PRODUCERS      = 3;
static const int PRINT_INTERVAL_MS  = 400;

// ── Shared state ──────────────────────────────────────────────────────────────
std::deque<std::string> buffer;
std::atomic<int>  produced_pairs{0};
std::atomic<int>  packaged_pairs{0};
std::atomic<bool> error_flag{false};

// ── POSIX Semaphores ──────────────────────────────────────────────────────────
sem_t empty_pairs;   // counts free pair-slots  (init: 50)
sem_t full_pairs;    // counts ready pairs       (init:  0)
sem_t mtx;           // binary mutex             (init:  1)

// ── Random sleep helper ───────────────────────────────────────────────────────
void rand_sleep(int lo_ms, int hi_ms) {
    thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(lo_ms, hi_ms);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
}

// ── Abort helper ──────────────────────────────────────────────────────────────
void abort_sim(const std::string& msg) {
    error_flag = true;
    std::cout << "\n!!! ERROR: " << msg << " !!!\n\n" << std::flush;
}

// ── Producer ──────────────────────────────────────────────────────────────────
void producer(int machine_id) {
    int pair_id = 0;
    while (!error_flag) {
        ++pair_id;
        std::string p1 = "M" + std::to_string(machine_id)
                        + "-" + std::to_string(pair_id) + "-P1";
        std::string p2 = "M" + std::to_string(machine_id)
                        + "-" + std::to_string(pair_id) + "-P2";

        rand_sleep(10, 40);   // simulate production time

        // ── Wait for a free pair-slot ─────────────────────────────────────
        sem_wait(&empty_pairs);
        if (error_flag) return;

        // ── Critical section ──────────────────────────────────────────────
        sem_wait(&mtx);
        if ((int)buffer.size() + 2 > BUFFER_CAPACITY) {
            abort_sim("The producing machine is broken");
            sem_post(&mtx);
            return;
        }
        buffer.push_back(p1);
        buffer.push_back(p2);
        ++produced_pairs;
        sem_post(&mtx);
        // ── End critical section ──────────────────────────────────────────

        // Signal: one more complete pair is ready for the consumer
        sem_post(&full_pairs);
    }
}

// ── Consumer ──────────────────────────────────────────────────────────────────
void consumer() {
    while (!error_flag) {
        // ── Wait for at least one complete pair ───────────────────────────
        sem_wait(&full_pairs);
        if (error_flag) return;

        // ── Critical section ──────────────────────────────────────────────
        sem_wait(&mtx);
        if ((int)buffer.size() < 2) {
            abort_sim("The packaging machine is broken");
            sem_post(&mtx);
            return;
        }

        std::string item1 = buffer.front(); buffer.pop_front();
        std::string item2 = buffer.front(); buffer.pop_front();

        // Verify pair: strip "-P1" / "-P2" suffix and compare prefixes
        // e.g. "M2-17-P1" → prefix "M2-17"
        std::string prefix1 = item1.substr(0, item1.rfind('-'));
        std::string prefix2 = item2.substr(0, item2.rfind('-'));

        if (prefix1 != prefix2) {
            abort_sim("Pairs are incorrect");
            std::cout << "  Mismatched: " << item1 << " + " << item2 << "\n"
                      << std::flush;
            sem_post(&mtx);
            return;
        }

        ++packaged_pairs;
        sem_post(&mtx);
        // ── End critical section ──────────────────────────────────────────

        // Signal: one pair-slot is now free for a producer
        sem_post(&empty_pairs);

        rand_sleep(10, 30);   // simulate packaging time
    }
}

// ── Status printer ────────────────────────────────────────────────────────────
void status_printer() {
    while (!error_flag) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(PRINT_INTERVAL_MS));

        sem_wait(&mtx);
        int pp  = produced_pairs.load();
        int pk  = packaged_pairs.load();
        int buf = (int)buffer.size();
        sem_post(&mtx);

        std::cout << "Produced pairs: " << pp
                  << " | Packaged pairs: " << pk
                  << " | Buffer particles: " << buf << "\n"
                  << std::flush;
    }
}

// ── Main ──────────────────────────────────────────────────────────────────────
int main() {
    // Initialise POSIX semaphores (pshared=0 means thread-shared, not process)
    sem_init(&empty_pairs, 0, BUFFER_CAPACITY / 2);  // 50 free pair-slots
    sem_init(&full_pairs,  0, 0);                     // 0 ready pairs
    sem_init(&mtx,         0, 1);                     // unlocked mutex

    std::cout << "=== AFTER SEMAPHORES: Safe Particle Buffer ===\n";
    std::cout << "  Producers : " << NUM_PRODUCERS << "\n";
    std::cout << "  Buffer    : " << BUFFER_CAPACITY
              << " particles (" << BUFFER_CAPACITY / 2 << " pairs)\n";
    std::cout << "  Press Ctrl+C to stop.\n\n" << std::flush;

    std::vector<std::thread> threads;

    for (int i = 1; i <= NUM_PRODUCERS; ++i)
        threads.emplace_back(producer, i);

    threads.emplace_back(consumer);
    threads.emplace_back(status_printer);

    for (auto& t : threads) t.join();

    std::cout << "[Main] Final — Produced: " << produced_pairs
              << " | Packaged: " << packaged_pairs
              << " | Buffer: " << buffer.size() << "\n";

    // Clean up semaphores
    sem_destroy(&empty_pairs);
    sem_destroy(&full_pairs);
    sem_destroy(&mtx);

    return 0;
}