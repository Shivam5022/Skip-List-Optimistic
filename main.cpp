#include <iostream>
#include <thread>
#include <vector>
#include <utility>
#include <format>
#include <chrono>
#include <random>
#include <mutex>

#include "skiplist.hpp"

constexpr int keyRange = 200000;
constexpr int operations = 100000;
constexpr int ADD = 9;
constexpr int REMOVE = 1;
constexpr int CONTAINS = 90;

void testThroughput(OptimisticSkipList skipList) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, keyRange);

    for (int i = 0; i < operations; ++i) {
        int key = dis(gen);
        int operation = dis(gen) % 100; // Determine operation type (add, remove, contains)
        if (operation < ADD) {
            skipList.add(key);
        } else if (operation < (ADD + REMOVE)) {
            skipList.remove(key);
        } else {
            skipList.contains(key);
        }
    }
}

void testAndPlot() {

    std::vector<int> numThreads = {1, 2, 3, 4, 5, 6, 7, 8};

    std::vector<std::pair<int, int>> plot;

    for (auto f: numThreads) {
        OptimisticSkipList skiplist;
        // spawn f threads, each doing random operation according to our distribution!
        std::vector<std::thread> threads;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < f; i++) {
            threads.emplace_back(testThroughput, skiplist);
        }
        for (auto& thread : threads) {
            thread.join();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        double throughput = (f * operations) / duration;
        // std::cout << "Throughput for " << f << " threads with range of keys " << keyRange << ": " << throughput << " ops/ms" << std::endl;
        plot.emplace_back(f, throughput);
    }

    std::cout << std::format("Throughput in operations per millisecond of {} operations, with {}% ‘add’, {}% ‘remove’, and {}% ‘contains’ operations. With key in range [0, {}]", operations, ADD, REMOVE, CONTAINS, keyRange) << std::endl;
    std::cout << "Number of Threads vs Throughput (in operations per milliseconds): \n";
    for (auto [x, y]: plot) {
        std::cout << x << ' ' << y << '\n';
    }
    std::cout << "[";
    for (int i = 0; i < (int) plot.size(); i++) {
        std::cout << plot[i].second << ", "[i == ((int) plot.size() - 1)];
    }
    std::cout << "]\n";
    return;
}

void skiplistTest() {
    std::vector<int> v;
    for (int i = 0; i < 100; i++) {
        v.emplace_back(i);
    }
    OptimisticSkipList skiplist;
    // Function to add random integers to the skip list
    auto addFunc = [&]() {
        for (int i = 1; i <= 100; ++i) {
            int val = i + 1;
            skiplist.add(val);
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Simulate some processing time
        }
    };

    // Function to remove random integers from the skip list
    auto removeFunc = [&]() {
        for (int i = 99; i >= 1; i--) {
            int val = i;
            skiplist.remove(val);
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Simulate some processing time
        }
    };

    std::thread addThread(addFunc);
    std::thread removeThread(removeFunc);

    addThread.join();
    removeThread.join();

    // removeFunc();
    // addFunc();

    int bad = 90;
    std::cout << "Contains " << bad << "? " << skiplist.contains(bad) << std::endl;
}

int main () {
    testAndPlot();
}

