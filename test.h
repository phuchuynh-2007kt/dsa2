/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AVL.h
 * Author: LTSACH
 *
 * Created on 12 September 2020, 17:11
 */

#ifndef AVL_H
#define AVL_H
#include "BST.h"
#include "support/DLinkedList.h"
#include "support/Queue.h"

#define XNode typename BST<K,V>::Node


template<class K, class V>
class AVL : public BST<K,V>{
public:
    void add(K key, V value=0){
        // Guard duplicate before allocating — search is O(log n)
        bool found = false;
        this->search(key, found);
        if (found) return;  // duplicate: caller keeps value

        XNode* newNode = new XNode(typename BST<K,V>::Entry(key, value));
        this->pRoot = add(this->pRoot, newNode);
        this->count++;
    }

    V remove(K key, bool* success=0){
        V retValue = 0; //O: NULL
        bool success_ = false;
        this->pRoot = remove(this->pRoot, key, success_, retValue);
        if(success_) this->count -= 1;
        //if(success != 0) if (success != 0) *success = success_;
        if (success != 0) *success = success_;
        return retValue;
    }
    int height(){
        if(this->pRoot == 0) return 0;
        return this->pRoot->height();
    }
    void println(string (*entry2str)(K&,V&)=0){
        cout << "Tree height: " << this->height() << endl;
        cout << "Tree nodes:"  << endl;
        cout << this->toString(entry2str, true) << endl;
    }
    /*
     * bfactor: designed for being used to test AVL tree
     *          so, maybe not useful for real application
     * It returns a list of balance factor (string) of nodes visited with BFS-traversal
     * (see AVLTest for example)
     */
    DLinkedList<string> bfactor(){
        DLinkedList<string> list;
        if(this->pRoot == 0) return list;
        
        Queue<XNode*> queue;
        queue.push(this->pRoot);
        while(!queue.empty()){
            XNode* pNode = queue.pop();
            if(pNode->bfactor == nEMPTY) list.add("EMPTY");
            else if(pNode->bfactor == nLH) list.add("LH");
            else if(pNode->bfactor == nEH) list.add("EH");
            else list.add("RH");
            
            if(pNode->bfactor == nEMPTY){
                delete pNode; //because alloc in [1] or [2]
                continue;
            } 
            if((pNode->pLeft == 0) && (pNode->pRight == 0) ) continue;
            
            if(pNode->pLeft != 0) queue.push(pNode->pLeft);
            else queue.push(new XNode()); //[1]
            if(pNode->pRight != 0) queue.push(pNode->pRight);
            else queue.push(new XNode()); //[2]
        }
        return list;
    }
private:
    // TODO Q1b: implement rotations, rebalancing, and recursive updates.
    // Node::updateHeight() and Node::bfactor are supplied infrastructure.
    /*
     * rotateLeft: right child becomes new root.
     *
     *      P              R
     *     / \            / \
     *   PL   R    =>    P   RR
     *       / \        / \
     *      RL  RR    PL  RL
     */
    XNode* rotateLeft(XNode* root){
        XNode* p = root;
        XNode* r = root->pRight;
        p->pRight = r->pLeft;   // RL moves to P's right
        r->pLeft  = p;          // P drops to R's left
        p->updateHeight();
        r->updateHeight();
        return r;
    }

    /*
     * rotateRight: left child becomes new root.
     *
     *      P              L
     *     / \            / \
     *    L   PR   =>   LL   P
     *   / \               / \
     *  LL  LR            LR  PR
     */
    XNode* rotateRight(XNode* root){
        XNode* p = root;
        XNode* l = root->pLeft;
        p->pLeft  = l->pRight;  // LR moves to P's left
        l->pRight = p;          // P drops to L's right
        p->updateHeight();
        l->updateHeight();
        return l;
    }

    /*
     * rebalance: called after every recursive step.
     * updateHeight() stores the ACTUAL height difference, so imbalance
     * appears as bfactor <= -2 or >= +2 (not -1/+1).
     * Child bfactor <= 0 covers both LL and the EH case that arises
     * during deletion; >= 0 likewise covers RR + deletion EH.
     */
    XNode* rebalance(XNode* root){
        if (root == nullptr) return root;

        if (root->bfactor <= -2) {          // left-heavy
            XNode* lc = root->pLeft;
            if (lc->bfactor <= 0)           // LL (or EH after deletion)
                root = rotateRight(root);
            else {                          // LR
                root->pLeft = rotateLeft(root->pLeft);
                root = rotateRight(root);
            }
        } else if (root->bfactor >= 2) {   // right-heavy
            XNode* rc = root->pRight;
            if (rc->bfactor >= 0)           // RR (or EH after deletion)
                root = rotateLeft(root);
            else {                          // RL
                root->pRight = rotateRight(root->pRight);
                root = rotateLeft(root);
            }
        }
        return root;
    }

    /*
     * AVL recursive insert.
     * Mirrors BST<K,V>::add(Node*, Entry) but:
     *   - accepts a pre-allocated node (no allocation inside)
     *   - calls rebalance() after updateHeight() on the way back up
     * Duplicate detection is handled by the public add() before allocation,
     * so this function assumes newNode->data.key is not already present.
     */
    XNode* add(XNode* root, XNode* newNode){
        if (root == nullptr) return newNode;

        if (newNode->data.key < root->data.key)
            root->pLeft  = add(root->pLeft,  newNode);
        else if (newNode->data.key > root->data.key)
            root->pRight = add(root->pRight, newNode);
        // equal: duplicate — should not reach here (public add guards it)

        root->updateHeight();
        return rebalance(root);
    }

    /*
     * AVL recursive remove.
     * Same ownership contract as BST: sets data.value = nullptr before
     * deleting a node so ~Node() does NOT free the managed pointer;
     * the pointer is returned via retValue for the caller to own.
     * Rebalances on the way back up after each recursive step.
     * Uses Inorder Successor (smallest of right subtree) for two-child case.
     */
    XNode* remove(XNode* root, K key, bool& success, V& retValue){
        if (root == nullptr) {
            success = false;
            return nullptr;
        }

        if (key < root->data.key)
            root->pLeft  = remove(root->pLeft,  key, success, retValue);
        else if (key > root->data.key)
            root->pRight = remove(root->pRight, key, success, retValue);
        else {
            // ── Case: no left child (covers leaf too) ──────────────────
            if (root->pLeft == nullptr) {
                XNode* right = root->pRight;
                retValue = root->data.value;
                root->data.value = nullptr; // prevent ~Node() from double-freeing
                delete root;
                success = true;
                return right;              // right may be nullptr (leaf case)
            }
            // ── Case: no right child ───────────────────────────────────
            if (root->pRight == nullptr) {
                XNode* left = root->pLeft;
                retValue = root->data.value;
                root->data.value = nullptr;
                delete root;
                success = true;
                return left;
            }
            // ── Case: two children — replace with Inorder Successor ────
            // Find min of right subtree
            XNode* succ = root->pRight;
            while (succ->pLeft != nullptr) succ = succ->pLeft;

            // Hand root's value to caller; take successor's key+value into root
            retValue             = root->data.value;
            root->data.key       = succ->data.key;
            root->data.value     = succ->data.value;

            // Remove successor from right subtree.
            // The recursive call will null succ->data.value before deleting it,
            // so root (which now holds the same pointer) is unaffected.
            bool dummyOk  = false;
            V    dummyVal = nullptr;
            root->pRight = remove(root->pRight, succ->data.key, dummyOk, dummyVal);
            success = true;
        }

        root->updateHeight();
        return rebalance(root);
    }
};

#endif /* AVL_H */
