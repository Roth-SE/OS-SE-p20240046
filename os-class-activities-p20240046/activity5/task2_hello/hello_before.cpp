/*
 * Task 2A: Print HELLO - BEFORE Semaphores (Unpredictable Order)
 *
 * Compile:  g++ -std=c++20 -pthread -o hello_before hello_before.cpp
 * Run:      ./hello_before
 *
 * Three threads print letters without any ordering coordination.
 * Output order is non-deterministic — expect scrambled results.
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <random>

void rand_sleep(int lo_ms, int hi_ms) {
    thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(lo_ms, hi_ms);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
}

// Process 1: prints H and E
void process1() {
    rand_sleep(0, 50);          // random delay — may run last
    std::cout << "H" << std::flush;
    rand_sleep(0, 20);
    std::cout << "E" << std::flush;
}

// Process 2: prints L and L
void process2() {
    rand_sleep(0, 50);
    std::cout << "L" << std::flush;
    rand_sleep(0, 20);
    std::cout << "L" << std::flush;
}

// Process 3: prints O
void process3() {
    rand_sleep(0, 50);
    std::cout << "O" << std::flush;
}

int main() {
    std::cout << "=== BEFORE SEMAPHORES: Printing HELLO without ordering ===\n";
    std::cout << "Running 5 times to show unpredictable output:\n\n";

    for (int run = 1; run <= 5; ++run) {
        std::cout << "Run " << run << ": " << std::flush;

        std::thread t1(process1);
        std::thread t2(process2);
        std::thread t3(process3);

        t1.join();
        t2.join();
        t3.join();

        std::cout << "\n";
    }

    std::cout << "\n[Done] Notice the letters are often in the wrong order!\n";
    return 0;
}