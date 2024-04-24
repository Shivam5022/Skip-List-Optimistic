#include <iostream>
#include <thread>
#include <vector>
#include <utility>
#include <chrono>
#include <random>
#include <mutex>
#include <fstream>
#include <memory>

#include "skiplist.hpp"

constexpr int ADD = 9;
constexpr int REMOVE = 1;
constexpr int CONTAINS = 90;

void runOperations (OptimisticSkipList skipList, int tid) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 500);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::string op;

    for (int i = 0; i < 10; ++i) {
        int key = dis(gen);
        int operation = dis(gen) % 100; // Determine operation type (add, remove, contains)
        if (operation < ADD) {
            skipList.add(key);
            op = "ADD";
        } else if (operation < (ADD + REMOVE)) {
            skipList.remove(key);
            op = "REMOVE";
        } else {
            skipList.contains(key);
            op = "CONTAINS";
        }

        // this print should be protected with lock! but skipping it.
        std::cout << "Thread " << tid << " performed " << op << std::endl;
    }
}

int main () {

    OptimisticSkipList skiplist;
    std::vector<std::thread> threads;

    const int numThreads = 10;

    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back(runOperations, skiplist, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
