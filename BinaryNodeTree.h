//
// Created by Raymond Heinzelman on 2019-03-24.
//

#ifndef PROJ4_BINARYNODETREE_H
#define PROJ4_BINARYNODETREE_H

#include "BinaryNode.h"

#include <iostream>
using namespace std;

template<class ItemType>
class BinaryNodeTree{

private:
    BinaryNode<ItemType>* rootPtr;

protected:
    int getHeightHelper(BinaryNode<ItemType>* subTreePtr) const;
    int getNumberOfNodesHelper(BinaryNode<ItemType>* subTreePtr) const;

    BinaryNode<ItemType>* balancedAdd(BinaryNode<ItemType>* subTreePtr, BinaryNode<ItemType>* newNodePtr);

    //needs to be virtual
    BinaryNode<ItemType>* removeValue(BinaryNode<ItemType>* subTreePtr, const ItemType target, bool& isSuccessful);

    BinaryNode<ItemType>* moveValueUpTree(BinaryNode<ItemType>* subTreePtr);

    BinaryNode<ItemType>* findNode(BinaryNode<ItemType>* treePtr, const ItemType& target, bool& isSuccessful) const;

    BinaryNode<ItemType>* copyTree(BinaryNode<ItemType>* oldTreeRootPtr) const;

    void destroyTree(BinaryNode<ItemType>* subTreePtr);

    void preorder(void visit(ItemType&), BinaryNode<ItemType>* treePtr) const;
    void inorder(void visit(ItemType&), BinaryNode<ItemType>* treePtr) const;
    void postorder(void visit(ItemType&), BinaryNode<ItemType>* treePtr) const;

public:
    BinaryNodeTree();
    BinaryNodeTree(const ItemType& rootItem);
    BinaryNodeTree(const ItemType& rootItem, const BinaryNode<ItemType>* leftTreePtr, const BinaryNode<ItemType>* rightTreePtr);
    BinaryNodeTree(const BinaryNode<ItemType>& tree);
    ~BinaryNodeTree();

    bool isEmpty() const;
    int getHeight() const;
    int getNumberOfNodes() const;
    //ItemType getRootData() const throw(PrecondViolatedExcept);
    bool add(const ItemType& newData); // Adds an item to the tree
    bool remove(const ItemType& data); // Removes specified item from the tree
    void clear();
    //ItemType getEntry(const ItemType& anEntry) const throw(NotFoundException);
    bool contains(const ItemType& anEntry) const;

};


//Constructors and Destructors
template<class ItemType>
BinaryNodeTree<ItemType>::BinaryNodeTree() {
    rootPtr = nullptr;
}

template<class ItemType>
BinaryNodeTree<ItemType>::BinaryNodeTree(const ItemType &rootItem) : rootPtr(BinaryNode<ItemType>(rootItem, nullptr, nullptr)) {
}

template<class ItemType>
BinaryNodeTree<ItemType>::BinaryNodeTree(const ItemType &rootItem, const BinaryNode<ItemType> *leftTreePtr,
                                         const BinaryNode<ItemType> *rightTreePtr)
                                         :  rootPtr(BinaryNode<ItemType>(rootItem, copyTree(leftTreePtr->rootPtr), copyTree(rightTreePtr->rootPtr))){}

//this might be a little sketch
template<class ItemType>
BinaryNodeTree<ItemType>::BinaryNodeTree(const BinaryNode<ItemType> &tree) {
    rootPtr = copyTree(tree.rootPtr);
}

template<class ItemType>
BinaryNodeTree<ItemType>::~BinaryNodeTree() {
    destroyTree(rootPtr);
}

//Class methods
template<class ItemType>
bool BinaryNodeTree<ItemType>::isEmpty() const {
    return (rootPtr == nullptr);
}

template<class ItemType>
int BinaryNodeTree<ItemType>::getHeight() const {
    return getHeightHelper(rootPtr);
}

template<class ItemType>
int BinaryNodeTree<ItemType>::getNumberOfNodes() const {
    return getNumberOfNodesHelper(rootPtr);
}

template<class ItemType>
bool BinaryNodeTree<ItemType>::add(const ItemType &newData) {
    BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newData);
    rootPtr = balancedAdd (rootPtr, newNodePtr);
    return true;
}

template<class ItemType>
bool BinaryNodeTree<ItemType>::remove(const ItemType &data) {
    bool removed = false;
    rootPtr = removeValue(rootPtr, data, removed);
    return true;
}

template<class ItemType>
void BinaryNodeTree<ItemType>::clear() {
    destroyTree(rootPtr);
    rootPtr = nullptr;
}

template<class ItemType>
bool BinaryNodeTree<ItemType>::contains(const ItemType &anEntry) const {
    bool found = false;
    findNode(rootPtr, anEntry, found);
    return found;
}

//protected functions
template<class ItemType>
int BinaryNodeTree<ItemType>::getHeightHelper(BinaryNode<ItemType> *subTreePtr) const {
    if (subTreePtr == nullptr)
        return 0;
    else
        return 1 + max(getHeightHelper(subTreePtr->getLeftChildPtr()),
                       getHeightHelper(subTreePtr->getRightChildPtr()));
}

template<class ItemType>
int BinaryNodeTree<ItemType>::getNumberOfNodesHelper(BinaryNode<ItemType> *subTreePtr) const {
    if ( subTreePtr == nullptr )
        return 0;
    else
    {
        return  1 + getNumberOfNodesHelper(subTreePtr->getLeftChildPtr()) + getNumberOfNodesHelper(subTreePtr->getRightChildPtr()) ;
    }
}

template<class ItemType>
BinaryNode<ItemType>* BinaryNodeTree<ItemType>::balancedAdd(BinaryNode<ItemType> *subTreePtr, BinaryNode<ItemType> *newNodePtr) {
    if (subTreePtr == nullptr) {
        return newNodePtr;
    }else {
        BinaryNode<ItemType> *leftPtr = subTreePtr->getLeftChildPtr();
        BinaryNode<ItemType> *rightPtr = subTreePtr->getRightChildPtr();
        if (getHeightHelper(leftPtr) > getHeightHelper(rightPtr)) {
            rightPtr = balancedAdd(rightPtr, newNodePtr);
            subTreePtr->setRightChildPtr(rightPtr);
        } else {
            leftPtr = balancedAdd(leftPtr, newNodePtr);
            subTreePtr->setLeftChildPtr(leftPtr);
        } // end if
        return subTreePtr;
    }
}

template<class ItemType>
BinaryNode<ItemType>* BinaryNodeTree<ItemType>::removeValue(BinaryNode<ItemType> *subTreePtr, const ItemType target,
                                                            bool &isSuccessful) {
    if(subTreePtr == nullptr){
        isSuccessful = false;
        return nullptr;
    } else if(subTreePtr->getItem() == target){
        subTreePtr = moveValueUpTree(subTreePtr);
        isSuccessful = true;
        return subTreePtr;
    } else{
        BinaryNode<ItemType>* tempA = removeValue(subTreePtr->getLeftChildPtr(), target, isSuccessful);
        BinaryNode<ItemType>* tempB = removeValue(subTreePtr->getRightChildPtr(), target, isSuccessful);
        if(tempA == nullptr){
            return tempB;
        } else{
            return tempA;
        }
    }
}

template<class ItemType>
BinaryNode<ItemType>* BinaryNodeTree<ItemType>::moveValueUpTree(BinaryNode<ItemType> *subTreePtr) {
    BinaryNode<ItemType>* temp;
    if(subTreePtr->isLeaf()){
        delete subTreePtr;
        subTreePtr = nullptr;
        return nullptr;
    } else if(subTreePtr->getLeftChildPtr() == nullptr && subTreePtr->getRightChildPtr() != nullptr){
        temp = subTreePtr->getRightChildPtr();
        delete subTreePtr;
        subTreePtr = nullptr;
        return temp;
    } else if(subTreePtr->getLeftChildPtr() != nullptr && subTreePtr->getRightChildPtr() == nullptr){
        temp = subTreePtr->getLeftChildPtr();
        delete subTreePtr;
        subTreePtr = nullptr;
        return temp;
    } else{
        temp = subTreePtr->getLeftChildPtr();
        subTreePtr->setItem(temp->getItem());
        moveValueUpTree(temp);
    }
}

template<class ItemType>
BinaryNode<ItemType>* BinaryNodeTree<ItemType>::findNode(BinaryNode<ItemType> *treePtr, const ItemType &target,
                                                         bool &isSuccessful) const {
    if (treePtr == nullptr)
        return nullptr; // Not found
    else if (treePtr->getItem() == target)
        return treePtr; // Found
    else if (treePtr->getItem() > target)
// Search left subtree
        return findNode(treePtr->getLeftChildPtr(), target);
    else
// Search right subtree
        return findNode(treePtr->getRightChildPtr(), target);
}

template<class ItemType>
BinaryNode<ItemType>* BinaryNodeTree<ItemType>::copyTree(BinaryNode<ItemType> *oldTreeRootPtr) const {
    std::shared_ptr<BinaryNode<ItemType>> newTreePtr;
// Copy tree nodes during a preorder traversal
    if (oldTreeRootPtr != nullptr)
    {
// Copy node
        newTreePtr = std::make_shared<BinaryNode<ItemType>>(oldTreeRootPtr->getItem(), nullptr, nullptr);
        newTreePtr->setLeftChildPtr(copyTree(oldTreeRootPtr->getLeftChildPtr()));
        newTreePtr->setRightChildPtr(copyTree(oldTreeRootPtr->getRightChildPtr()));
    } // end if
// Else tree is empty (newTreePtr is nullptr)
    return newTreePtr;
}

template<class ItemType>
void BinaryNodeTree<ItemType>::destroyTree(BinaryNode<ItemType> *subTreePtr) {
    if (subTreePtr != nullptr)
    {
        destroyTree(subTreePtr->getLeftChildPtr());
        destroyTree(subTreePtr->getRightChildPtr());
        delete subTreePtr; // Decrement reference count to node
    } // end if
}

//preorder(): traverses the BNT in preorder
template<class ItemType>
void BinaryNodeTree<ItemType>::preorder(void visit(ItemType &), BinaryNode<ItemType> *treePtr) const {
    if(treePtr != nullptr){
        ItemType item = treePtr->getItem();
        visit(item);
        preorder(visit, treePtr->getLeftChildPtr());
        preorder(visit, treePtr->getRightChildPtr());
    }
}

//inorder(): traverses the BNT in inorder
template<class ItemType>
void BinaryNodeTree<ItemType>::inorder(void visit(ItemType &), BinaryNode<ItemType> *treePtr) const {
    if(treePtr != nullptr){
        ItemType item = treePtr->getItem();
        inorder(visit, treePtr->getLeftChildPtr());
        visit(item);
        inorder(visit, treePtr->getRightChildPtr());
    }
}

//postorder(): traverses the BNT in postorder
template<class ItemType>
void BinaryNodeTree<ItemType>::postorder(void visit(ItemType &), BinaryNode<ItemType> *treePtr) const {
    if(treePtr != nullptr){
        ItemType item = treePtr->getItem();
        postorder(visit, treePtr->getLeftChildPtr());
        postorder(visit, treePtr->getRightChildPtr());
        visit(item);
    }
}

#endif //PROJ4_BINARYNODETREE_H
