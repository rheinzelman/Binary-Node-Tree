//
// Created by Raymond Heinzelman on 2019-03-16.
//

#ifndef PROJ4_BINARYNODE_H
#define PROJ4_BINARYNODE_H

#include <memory>

template<class ItemType>
class BinaryNode
{

private:
    ItemType item;

    BinaryNode<ItemType>* leftChildPtr;

    BinaryNode<ItemType>* rightChildPtr;

public:
    BinaryNode();
    BinaryNode(const ItemType& nodeItem);
    BinaryNode(const ItemType& nodeItem, BinaryNode<ItemType>* leftPtr, BinaryNode<ItemType>* rightPtr);

    void setItem(const ItemType& anItem);
    ItemType getItem() const;
    bool isLeaf() const;

    BinaryNode<ItemType>* getLeftChildPtr() const;
    BinaryNode<ItemType>* getRightChildPtr() const;

    void setLeftChildPtr(BinaryNode<ItemType>* leftPtr);
    void setRightChildPtr(BinaryNode<ItemType>* rightPtr);

};

//Default ctor
template<class ItemType>
BinaryNode<ItemType>::BinaryNode() {
    leftChildPtr = nullptr;
    rightChildPtr = nullptr;
}

//parameterized ctor
template<class ItemType>
BinaryNode<ItemType>::BinaryNode(const ItemType &nodeItem) {
    item = nodeItem;
    leftChildPtr = nullptr;
    rightChildPtr = nullptr;
}

//parameterized ctor
template<class ItemType>
BinaryNode<ItemType>::BinaryNode(const ItemType &nodeItem, BinaryNode<ItemType> *leftPtr,
                                 BinaryNode<ItemType> *rightPtr) {
    item = nodeItem;
    leftChildPtr = leftPtr;
    rightChildPtr = rightPtr;
}

//setItem(): sets the value of a nodes item
template<class ItemType>
void BinaryNode<ItemType>::setItem(const ItemType &anItem) {
    item = anItem;
}

//getItem(): returns the value of a nodes item
template<class ItemType>
ItemType BinaryNode<ItemType>::getItem() const {
    return item;
}

//isLeaf(): returns true if the node is a leaf, false if not
template<class ItemType>
bool BinaryNode<ItemType>::isLeaf() const {
    if(leftChildPtr == nullptr && rightChildPtr == nullptr)
        return true;
    else
        return false;
}

//getLeftChildPtr(): returns a pointer to a node's left child
template <class ItemType>
BinaryNode<ItemType>* BinaryNode<ItemType>::getLeftChildPtr() const {
    return leftChildPtr;
}

//getrightChildPtr(): returns a pointer to a node's right child
template <class ItemType>
BinaryNode<ItemType>* BinaryNode<ItemType>::getRightChildPtr() const {
    return rightChildPtr;
}

//setLeftChildPtr(): sets a node's left child pointer
template<class ItemType>
void BinaryNode<ItemType>::setLeftChildPtr(BinaryNode<ItemType> *leftPtr) {
    leftChildPtr = leftPtr;
}

//setRightChildPtr(): sets a node's right child pointer
template<class ItemType>
void BinaryNode<ItemType>::setRightChildPtr(BinaryNode<ItemType> *rightPtr) {
    rightChildPtr = rightPtr;
}

#endif //PROJ4_BINARYNODE_H
