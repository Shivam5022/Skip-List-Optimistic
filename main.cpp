#include <iostream>
#include <thread>

#include "skiplist.hpp"

int main() {
    // Example usage
    // OptimisticSkipList skiplist;

    // for (int i = 0; i < 40; i++) {
    //     skiplist.add(i);
    // }

    // std::cout << "contains 5? " << skiplist.contains(5) << std::endl;
    // std::cout << "contains 10? " << skiplist.contains(10) << std::endl;
    // std::cout << "contains 3? " << skiplist.contains(3) << std::endl;
    // std::cout << "contains 70? " << skiplist.contains(70) << std::endl;

    // skiplist.remove(30);
    // std::cout << "contains 30? " << skiplist.contains(30) << std::endl;

    // return 0;


    OptimisticSkipList skiplist;
    // Function to add random integers to the skip list
    auto addFunc = [&]() {
        for (int i = 0; i < 10; ++i) {
            int val = int(rand() % 1000);
            skiplist.add(val);
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Simulate some processing time
        }
    };

    // Function to remove random integers from the skip list
    auto removeFunc = [&]() {
        for (int i = 0; i < 1000; ++i) {
            int val = int(rand() % 1000);
            skiplist.remove(val);
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Simulate some processing time
        }
    };

    std::thread addThread(addFunc);
    std::thread removeThread(removeFunc);

    addThread.join();
    removeThread.join();

    std::cout << "Contains 30? " << skiplist.contains(30) << std::endl;
}

