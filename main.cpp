#include <iostream>
#include "BKMove.h"
#include "BST.h"
#include "AVL.h"
#include "Heap.h"

int main() {
    // The starter compiles before any TODOs are filled in.
    // Returned values here are placeholders, not expected final results.
    BKMove app;
    StopInfo stop{"S001", "Dai hoc Bach Khoa", "dai hoc bach khoa"};
    std::cout << "BKMove Assignment 2 (starter skeleton)\n";
    std::cout << "addStop: " << app.addStop(stop) << '\n';
    std::cout << "stop count: " << app.getStopCount() << '\n';
    std::cout << "search results: " << app.findStops("dai", 0, 10).size() << '\n';
    BST<int, int*> bst;
    AVL<int, int*> avl;
    Heap<int> heap;
    std::cout << "BST size: " << bst.size()
              << ", AVL size: " << avl.size()
              << ", Heap size: " << heap.size() << '\n';
}
