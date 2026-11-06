#include "tests/TestUtils.h"
#include "Heap.h"
#include <stdexcept>
#include <vector>

static int compares = 0;
static int byInt(int& a, int& b) { ++compares; return (a > b) - (a < b); }
static int byDescending(int& a, int& b) { return (b > a) - (b < a); }
static int callbacks = 0;
static void onRemove(int) { ++callbacks; }

int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q2 - Binary Heap");
    suite.add("Empty state and underflow", [] {
        Heap<int> heap;
        CHECK(heap.empty() && heap.size() == 0);
        bool thrown = false;
        try { heap.peek(); } catch (const std::underflow_error&) { thrown = true; }
        CHECK(thrown);
        thrown = false;
        try { heap.pop(); } catch (const std::underflow_error&) { thrown = true; }
        CHECK(thrown);
    });
    suite.add("Min-heap insertion and peek", [] {
        Heap<int> heap;
        for (int x : {5, 2, 9, 1, 3}) heap.push(x);
        CHECK(heap.size() == 5 && heap.peek() == 1);
        CHECK(heap.size() == 5);
    });
    suite.add("Pop in ascending order", [] {
        Heap<int> heap;
        for (int x : {5, 2, 9, 1, 3}) heap.push(x);
        for (int x : {1, 2, 3, 5, 9}) CHECK(heap.pop() == x);
        CHECK(heap.empty());
    });
    suite.add("Custom comparator yields max-heap", [] {
        Heap<int> heap(byDescending);
        for (int x : {2, 7, 3, 10, 1}) heap.push(x);
        for (int x : {10, 7, 3, 2, 1}) CHECK(heap.pop() == x);
    });
    suite.add("Equal priorities and negative numbers", [] {
        Heap<int> heap;
        for (int x : {-2, 0, -2, 7, 0, -10}) heap.push(x);
        for (int x : {-10, -2, -2, 0, 0, 7}) CHECK(heap.pop() == x);
    });
    suite.add("Contains and remove existing item", [] {
        Heap<int> heap;
        for (int x : {4, 2, 7, 1, 9, 3}) heap.push(x);
        CHECK(heap.contains(7));
        callbacks = 0;
        heap.remove(7, onRemove);
        CHECK(!heap.contains(7) && callbacks == 1 && heap.size() == 5);
        for (int x : {1, 2, 3, 4, 9}) CHECK(heap.pop() == x);
    });
    suite.add("Removing absent item preserves heap", [] {
        Heap<int> heap;
        for (int x : {6, 2, 10}) heap.push(x);
        callbacks = 0;
        heap.remove(999, onRemove);
        CHECK(callbacks == 0 && heap.size() == 3);
        for (int x : {2, 6, 10}) CHECK(heap.pop() == x);
    });
    suite.add("Remove root and an interior item", [] {
        Heap<int> heap;
        for (int x : {8, 3, 5, 1, 9, 4, 7}) heap.push(x);
        heap.remove(1); heap.remove(5);
        CHECK(heap.size() == 5);
        for (int x : {3, 4, 7, 8, 9}) CHECK(heap.pop() == x);
    });
    suite.add("Bottom-up heapify is linear", [] {
        constexpr int N = 4096;
        std::vector<int> input;
        for (int i = N; i >= 1; --i) input.push_back(i);
        Heap<int> heap(byInt);
        compares = 0;
        heap.heapify(input.data(), N);
        CHECK(heap.size() == N);
        CHECK(compares < 4 * N);
        for (int i = 1; i <= N; ++i) CHECK(heap.pop() == i);
    });
    suite.add("Heapify replaces previous heap", [] {
        Heap<int> heap;
        heap.push(0); heap.push(-1);
        int values[] = {7, 4, 6, 5};
        heap.heapify(values, 4);
        CHECK(heap.size() == 4);
        for (int x : {4, 5, 6, 7}) CHECK(heap.pop() == x);
    });
    return suite.run(argc, argv);
}
