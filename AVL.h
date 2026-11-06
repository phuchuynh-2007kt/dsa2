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
    //guard against duplicate
    bool found = false;
    this->search(key, found);   //an unused value here
    if(found == true) return;

    XNode* newNode = new XNode{typename BST<K, V>::Entry{key, value}};
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
    XNode* rotateLeft(XNode* root){
        XNode* p = root;
        root = root->pRight;
        p->pRight = root->pLeft;
        root->pLeft = p;

        p->updateHeight();
        root->updateHeight();

        return root;
    }
    XNode* rotateRight(XNode* root){
        XNode* p = root;
        root = root->pLeft;
        p->pLeft = root->pRight;
        root->pRight = p;

        p->updateHeight();
        root->updateHeight();

        return root; 
    }
    XNode* rebalance(XNode* root){
        if (root == nullptr || (-1 <= root->bfactor && root->bfactor <= 1))
            return root;

        XNode* p = root;
        if(root->bfactor < -1) {
            p = p->pLeft;
            if (p->bfactor <= 0)
                root = rotateRight(root);
            else {
                root->pLeft = rotateLeft(root->pLeft);
                root = rotateRight(root);
            }
        } else {
            p = p->pRight;
            if (p->bfactor >= 0)
                root = rotateLeft(root);
            else {
                root->pRight = rotateRight(root->pRight);
                root = rotateLeft(root);
            }
        }
        return root;
    }
    XNode* add(XNode* root, XNode* newNode){
        if(root == nullptr) return newNode;

        if(root->data.key < newNode->data.key) 
            root->pRight = add(root->pRight, newNode);
        else if(root->data.key > newNode->data.key)
            root->pLeft = add(root->pLeft, newNode);
        
        root->updateHeight();
        root = rebalance(root);
        return root; 
    }
    XNode* remove(XNode* root, K key, bool& success, V& retValue){
        if(root == nullptr) {
            success = false;
            return nullptr;
        }

        if (key < root->data.key)
            root->pLeft  = remove(root->pLeft,  key, success, retValue);
        else if (key > root->data.key)
            root->pRight = remove(root->pRight, key, success, retValue);
        else {
            if(root->pLeft == nullptr) {
                XNode* p = root->pRight;    
                retValue = root->data.value;
                root->data.value = nullptr;
                delete root;
                success = true;
                return p;
            }
            if(root->pRight == nullptr) {
                XNode* p = root->pLeft;
                retValue = root->data.value;
                root->data.value = nullptr; //requires more attention here
                delete root;
                success = true;
                return p;
            }

            XNode* newNode = root->pRight;
            while(newNode->pLeft != nullptr)
                newNode = newNode->pLeft;

            retValue = root->data.value;
            root->data.value = newNode->data.value;
            root->data.key = newNode->data.key;
            
            bool ignoredSuccess = false;
            V ignoredValue = nullptr;
            root->pRight = remove(root->pRight, newNode->data.key, ignoredSuccess, ignoredValue);
            success = true;
        }
        root->updateHeight();
        root = rebalance(root);
        return root;
    }
};

#endif /* AVL_H */
