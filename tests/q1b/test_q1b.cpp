#include "tests/TestUtils.h"
#include "AVL.h"
#include <vector>

static void assertRotation(const std::vector<int>& input) {
    AVL<int, int*> tree;
    for (int v : input) tree.add(v, new int(v));
    CHECK(tree.size() == 3 && tree.height() == 2);
    for (int v : input) {
        bool found = false;
        int* p = tree.search(v, found);
        CHECK(found && p && *p == v);
    }
}
int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q1b - AVL");
    suite.add("Empty tree and one insertion", [] {
        AVL<int, int*> tree;
        CHECK(tree.size() == 0 && tree.height() == 0);
        tree.add(11, new int(11));
        CHECK(tree.size() == 1 && tree.height() == 1);
    });
    suite.add("LL rotation", [] { assertRotation({30, 20, 10}); });
    suite.add("RR rotation", [] { assertRotation({10, 20, 30}); });
    suite.add("LR rotation", [] { assertRotation({30, 10, 20}); });
    suite.add("RL rotation", [] { assertRotation({10, 30, 20}); });
    suite.add("Ascending insertions maintain logarithmic height", [] {
        AVL<int, int*> tree;
        for (int i = 0; i < 255; ++i) tree.add(i, new int(i));
        CHECK(tree.size() == 255 && tree.height() <= 12);
        for (int i = 0; i < 255; i += 13) {
            bool found = false;
            int* p = tree.search(i, found);
            CHECK(found && p && *p == i);
        }
    });
    suite.add("Duplicate insertion is ignored", [] {
        AVL<int, int*> tree;
        tree.add(10, new int(10));
        int* rejected = new int(99);
        tree.add(10, rejected);
        CHECK(tree.size() == 1 && tree.height() == 1);
        delete rejected;
    });
    suite.add("Delete root and preserve remaining keys", [] {
        AVL<int, int*> tree;
        for (int v : {30, 20, 40, 10, 25, 35, 50}) tree.add(v, new int(v));
        bool ok = false;
        int* p = tree.remove(30, &ok);
        CHECK(ok && p && *p == 30); delete p;
        CHECK(tree.size() == 6 && tree.height() <= 4);
        for (int v : {20, 40, 10, 25, 35, 50}) {
            bool found = false;
            CHECK(tree.search(v, found) != nullptr && found);
        }
    });
    suite.add("Delete many nodes and rebalance", [] {
        AVL<int, int*> tree;
        for (int i = 0; i < 255; ++i) tree.add(i, new int(i * 3));
        for (int i = 0; i < 255; i += 3) {
            bool ok = false;
            int* p = tree.remove(i, &ok);
            CHECK(ok && p && *p == i * 3); delete p;
        }
        CHECK(tree.size() == 170 && tree.height() <= 12);
        for (int i = 0; i < 255; ++i) {
            bool found = false;
            int* p = tree.search(i, found);
            CHECK(found == (i % 3 != 0)); CHECK((p != nullptr) == found);
        }
    });
    suite.add("Missing removal, optional success and clear", [] {
        AVL<int, int*> tree;
        for (int v : {10, 20, 30}) tree.add(v, new int(v));
        bool ok = true;
        CHECK(tree.remove(123, &ok) == nullptr && !ok);
        int* p = tree.remove(20);
        CHECK(p && *p == 20); delete p;
        tree.clear();
        CHECK(tree.size() == 0 && tree.height() == 0);
    });
    return suite.run(argc, argv);
}
