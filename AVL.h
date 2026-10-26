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
        // TODO Q1b: insert a new key, ignore duplicates, update count.
        (void)key; (void)value;
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
        return root; // TODO
    }
    XNode* rotateRight(XNode* root){
        return root; // TODO
    }
    XNode* rebalance(XNode* root){
        return root; // TODO
    }
    XNode* add(XNode* root, XNode* newNode){
        (void)newNode;
        return root; // TODO
    }
    XNode* remove(XNode* root, K key, bool& success, V& retValue){
        (void)key; (void)retValue;
        success = false;
        return root; // TODO
    }
};

#endif /* AVL_H */
