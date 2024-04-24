#pragma once

#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <mutex>
#include <fstream>

struct Node;

struct lSentinel {};
struct rSentinel {};

struct Node {
    int topLayer;
    bool fullyLinked;
    bool removed;
    std::mutex lock;
    std::vector<std::shared_ptr<Node>> nexts;
    int value;

    Node(int topLayer, bool fullyLinked, int value) :
        topLayer(topLayer), fullyLinked(fullyLinked), removed(false), value(value) {
        nexts.resize(topLayer + 1);
    }
};

// add, remove and contains ->
// add(v): adds v to the set and returns true iff v was not already in the set; 
// remove(v): removes v from the set and returns true iff v was in the set; 
// contains(v): returns true iff v is in the set;

class OptimisticSkipList {

private:
    std::shared_ptr<Node> head;
    const int maxHeight = 128;
    
    int findNode(int v, std::vector<std::shared_ptr<Node>>& preds, std::vector<std::shared_ptr<Node>>& succs) {
        auto pred = head;
        int found = -1;
        for (int layer = maxHeight - 1; layer >= 0; --layer) {
            auto curr = pred->nexts[layer];
            while (curr->value < v) {
                pred = curr;
                curr = pred->nexts[layer];
            }
            if (found == -1 && v == curr->value) {
                found = layer;
            }
            preds[layer] = pred;
            succs[layer] = curr;
        }
        return found;
    }

    int randomLevel(int foo) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, foo - 1);
        return dis(gen);
    }

    bool okToDelete(std::shared_ptr<Node> candidate, int l) {
        return candidate->fullyLinked && candidate->topLayer == l && !candidate->removed;
    }

    void unlock(std::vector<std::shared_ptr<Node>>& preds, int highestLocked) {
        std::shared_ptr<Node> prevPred, pred;
        for (int layer = 0; layer <= highestLocked; ++layer) {
            pred = preds[layer];
            if (pred != prevPred) {
                pred->lock.unlock();
            }
            prevPred = pred;
        }
    }

public:
    OptimisticSkipList() {
        auto h = std::make_shared<Node>(maxHeight, true, std::numeric_limits<int>::min());
        auto t = std::make_shared<Node>(maxHeight, true, std::numeric_limits<int>::max());

        for (int i = 0; i < maxHeight; ++i) {
            h->nexts[i] = t;
        }

        head = h;
    }

    bool add(int v) {
        int topLayer = randomLevel(maxHeight);
        std::vector<std::shared_ptr<Node>> preds(maxHeight);
        std::vector<std::shared_ptr<Node>> succs(maxHeight);
        while (true) {
            int found = findNode(v, preds, succs);
            if (found != -1) {
                auto nodeFound = succs[found];
                if (!nodeFound->removed) {
                    while (!nodeFound->fullyLinked) {}
                    return false;
                }
                continue;
            }
            int highestLocked = -1;
            std::shared_ptr<Node> pred, succ, prevPred = nullptr;
            bool valid = true;
            for (int layer = 0; valid && (layer <= topLayer); ++layer) {
                pred = preds[layer];
                succ = succs[layer];
                if (pred != prevPred) {
                    pred->lock.lock();
                    highestLocked = layer;
                    prevPred = pred;
                }
                valid = !pred->removed && !succ->removed && pred->nexts[layer] == succ;
            }
            if (!valid) {
                unlock(preds, highestLocked);
                continue;
            }

            auto newNode = std::make_shared<Node>(topLayer, false, v);

            for (int layer = 0; layer <= topLayer; ++layer) {
                newNode->nexts[layer] = succs[layer];
            }
            for (int layer = 0; layer <= topLayer; ++layer) {
                preds[layer]->nexts[layer] = newNode;
            }

            newNode->fullyLinked = true;
            unlock(preds, highestLocked);
            return true;
        }
    }

    bool remove(int v) {
        std::shared_ptr<Node> nodeToDelete;
        bool isremoved = false;
        int topLayer = -1;
        std::vector<std::shared_ptr<Node>> preds(maxHeight);
        std::vector<std::shared_ptr<Node>> succs(maxHeight);
        while (true) {
            int found = findNode(v, preds, succs);
            if (isremoved || (found != -1 && okToDelete(succs[found], found))) {
                if (!isremoved) {
                    nodeToDelete = succs[found];
                    topLayer = nodeToDelete->topLayer;
                    nodeToDelete->lock.lock();
                    if (nodeToDelete->removed) {
                        nodeToDelete->lock.unlock();
                        return false;
                    }
                    nodeToDelete->removed = true;
                    isremoved = true;
                }
                int highestLocked = -1;
                std::shared_ptr<Node> pred, succ, prevPred = nullptr;
                bool valid = true;
                for (int layer = 0; valid && (layer <= topLayer); ++layer) {
                    pred = preds[layer];
                    succ = succs[layer];
                    if (pred != prevPred) {
                        pred->lock.lock();
                        highestLocked = layer;
                        prevPred = pred;
                    }
                    valid = !pred->removed && pred->nexts[layer] == succ;
                }
                if (!valid) {
                    unlock(preds, highestLocked);
                    continue;
                }
                for (int layer = topLayer; layer >= 0; --layer) {
                    preds[layer]->nexts[layer] = nodeToDelete->nexts[layer];
                }

                nodeToDelete->lock.unlock();
                nodeToDelete.reset();
                
                unlock(preds, highestLocked);
                return true;
            } else {
                return false;
            }
        }
    }

    bool contains(int v) {
        std::vector<std::shared_ptr<Node>> preds(maxHeight);
        std::vector<std::shared_ptr<Node>> succs(maxHeight);
        int found = findNode(v, preds, succs);
        bool f =  ((found != -1) && (succs[found]->fullyLinked) && (!succs[found]->removed));
        return f;
    }
};
