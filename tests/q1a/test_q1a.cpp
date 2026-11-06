#include "tests/TestUtils.h"
#include "BST.h"
#include <string>

int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q1a - BST");
    suite.add("Empty tree and missing keys", [] {
        BST<int, int*> tree;
        CHECK(tree.empty()); CHECK(tree.size() == 0); CHECK(tree.height() == 0);
        bool found = true;
        CHECK(tree.search(123, found) == nullptr && !found);
        bool removed = true;
        CHECK(tree.remove(123, &removed) == nullptr && !removed);
    });
    suite.add("Insert and search seven nodes", [] {
        BST<int, int*> tree;
        for (int k : {50, 30, 70, 20, 40, 60, 80}) tree.add(k, new int(k * 2));
        CHECK(tree.size() == 7); CHECK(tree.height() == 3);
        for (int k : {50, 30, 70, 20, 40, 60, 80}) {
            bool found = false;
            int* ptr = tree.search(k, found);
            CHECK(found && ptr && *ptr == k * 2);
        }
    });
    suite.add("Duplicate key leaves original value unchanged", [] {
        BST<int, int*> tree;
        tree.add(8, new int(80));
        int* rejected = new int(800);
        tree.add(8, rejected);
        CHECK(tree.size() == 1);
        bool found = false;
        int* stored = tree.search(8, found);
        CHECK(found && stored && *stored == 80);
        delete rejected;
    });
    suite.add("Deleting a leaf transfers value ownership", [] {
        BST<int, int*> tree;
        for (int k : {10, 4, 16}) tree.add(k, new int(k));
        bool ok = false;
        int* p = tree.remove(4, &ok);
        CHECK(ok && p && *p == 4); delete p;
        CHECK(tree.size() == 2);
        bool found = true;
        CHECK(!tree.search(4, found) && !found);
    });
    suite.add("Deleting a node with one child", [] {
        BST<int, int*> tree;
        for (int k : {10, 4, 16, 12}) tree.add(k, new int(k));
        bool ok = false;
        int* p = tree.remove(16, &ok);
        CHECK(ok && p && *p == 16); delete p;
        CHECK(tree.size() == 3);
        bool found = false;
        p = tree.search(12, found);
        CHECK(found && p && *p == 12);
    });
    suite.add("Deleting a node with two children", [] {
        BST<int, int*> tree;
        for (int k : {50, 30, 70, 20, 40, 60, 80, 35, 45}) tree.add(k, new int(k));
        bool ok = false;
        int* p = tree.remove(30, &ok);
        CHECK(ok && p && *p == 30); delete p;
        CHECK(tree.size() == 8);
        for (int k : {50, 70, 20, 40, 60, 80, 35, 45}) {
            bool found = false;
            p = tree.search(k, found);
            CHECK(found && p && *p == k);
        }
    });
    suite.add("Removing the root repeatedly", [] {
        BST<int, int*> tree;
        for (int k : {4, 2, 6, 1, 3, 5, 7}) tree.add(k, new int(k));
        for (int k : {4, 5, 2, 6, 1, 3, 7}) {
            bool ok = false;
            int* p = tree.remove(k, &ok);
            CHECK(ok && p && *p == k); delete p;
        }
        CHECK(tree.empty() && tree.size() == 0 && tree.height() == 0);
    });
    suite.add("Optional success pointer and missing deletion", [] {
        BST<int, int*> tree;
        tree.add(3, new int(30));
        CHECK(tree.remove(999) == nullptr);
        int* p = tree.remove(3);
        CHECK(p && *p == 30); delete p;
        CHECK(tree.size() == 0);
    });
    suite.add("Clear and reuse a nonempty tree", [] {
        BST<int, int*> tree;
        for (int k = 0; k < 31; ++k) tree.add(k, new int(k));
        CHECK(tree.size() == 31); CHECK(tree.height() == 31);
        tree.clear();
        CHECK(tree.size() == 0 && tree.height() == 0 && tree.empty());
        tree.add(5, new int(7));
        bool found = false;
        int* p = tree.search(5, found);
        CHECK(found && p && *p == 7);
    });
    return suite.run(argc, argv);
}
