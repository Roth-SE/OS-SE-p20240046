/*
 * Task 1A: Particle Pair Buffer - BEFORE Semaphores (Intentionally Unsafe)
 *
 * Compile:  g++ -std=c++20 -pthread -o particles_before particles_before.cpp
 * Run:      ./particles_before
 *
 * Three producers and one consumer share a buffer with NO proper
 * synchronization. Expect overflow errors or mismatched pairs quickly.
 */

#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <string>
#include <chrono>
#include <atomic>
#include <random>
#include <sstream>

// ── Shared state (unprotected) ───────────────────────────────────────────────
static const int BUFFER_CAPACITY = 100;   // max particles = 50 pairs
static const int NUM_PRODUCERS   = 3;

std::deque<std::string> buffer;           // shared particle buffer (NO mutex)
std::atomic<int>  produced_pairs{0};
std::atomic<int>  packaged_pairs{0};
std::atomic<bool> error_flag{false};

// ── Small random sleep helper ────────────────────────────────────────────────
void rand_sleep(int lo_ms, int hi_ms) {
    thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(lo_ms, hi_ms);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
}

// ── Producer ─────────────────────────────────────────────────────────────────
void producer(int machine_id) {
    int pair_id = 0;
    while (!error_flag) {
        ++pair_id;
        std::string p1 = "M" + std::to_string(machine_id)
                        + "-" + std::to_string(pair_id) + "-P1";
        std::string p2 = "M" + std::to_string(machine_id)
                        + "-" + std::to_string(pair_id) + "-P2";

        rand_sleep(1, 5);   // simulate production time

        // ── NO mutex: race condition on size check ────────────────────────
        if ((int)buffer.size() + 2 > BUFFER_CAPACITY) {
            std::cout << "The producing machine is broken\n";
            error_flag = true;
            return;
        }

        buffer.push_back(p1);
        std::this_thread::yield();   // encourage interleaving between P1 and P2
        buffer.push_back(p2);

        ++produced_pairs;
    }
}

// ── Consumer ─────────────────────────────────────────────────────────────────
void consumer() {
    while (!error_flag) {
        rand_sleep(2, 7);

        // ── NO wait: may read empty buffer ────────────────────────────────
        if (buffer.size() < 2) {
            std::cout << "The packaging machine is broken\n";
            error_flag = true;
            return;
        }

        std::string item1 = buffer.front(); buffer.pop_front();
        std::this_thread::yield();          // another thread may modify here
        std::string item2 = buffer.front(); buffer.pop_front();

        // Verify pair: strip trailing "-P1" / "-P2" and compare prefixes
        std::string prefix1 = item1.substr(0, item1.rfind('-'));
        std::string prefix2 = item2.substr(0, item2.rfind('-'));

        if (prefix1 != prefix2) {
            std::cout << "Pairs are incorrect\n";
            std::cout << "  Got: " << item1 << " + " << item2 << "\n";
            error_flag = true;
            return;
        }

        ++packaged_pairs;
        std::cout << "Packaged: " << item1 << " + " << item2
                  << " | Produced: " << produced_pairs
                  << " | Packaged: "  << packaged_pairs
                  << " | Buffer: "    << buffer.size() << "\n";
    }
}

// ── Main ─────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "=== BEFORE SEMAPHORES: Unsafe Particle Buffer ===\n";
    std::cout << "Expect errors or incorrect pairs due to race conditions.\n\n";

    std::vector<std::thread> threads;

    for (int i = 1; i <= NUM_PRODUCERS; ++i)
        threads.emplace_back(producer, i);

    threads.emplace_back(consumer);

    for (auto& t : threads) t.join();

    std::cout << "\n[Main] Simulation ended.\n";
    return 0;
}