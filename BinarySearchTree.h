//
// Created by Raymond Heinzelman on 2019-03-24.
//

#include "BinaryTreeInterface.h"
#include "BinaryNode.h"
#include "BinaryNodeTree.h"
#include <memory>

template<class ItemType>
class BinarySearchTree : public BinaryNodeTree<ItemType>
{

private:
    BinaryNode<ItemType>* rootPtr;

protected:
    BinaryNode<ItemType>* placeNode(BinaryNode<ItemType>* subTreePtr, BinaryNode<ItemType>* newNode);

    BinaryNode<ItemType>* removeValue(BinaryNode<ItemType>* subTreePtr, const ItemType target, bool& isSuccessful);

    BinaryNode<ItemType>* removeNode(BinaryNode<ItemType>* nodePtr);

    BinaryNode<ItemType>* removeLeftmostNode(BinaryNode<ItemType>* subTreePtr, ItemType& inorderSuccessor);

    BinaryNode<ItemType>* findNode(BinaryNode<ItemType>* treePtr, const ItemType& target) const;

    BinaryNode<ItemType>* balancedAdd(BinaryNode<ItemType> *subTreePtr, BinaryNode<ItemType> *newNodePtr);

public:
    BinarySearchTree();
    BinarySearchTree(const ItemType& rootItem);
    BinarySearchTree(const BinarySearchTree<ItemType>& tree);
    ~BinarySearchTree();

    bool isEmpty() const;
    int getHeight() const;
    int getNumberOfNodes() const;

    void setRootData(const ItemType& newData);

    bool add(const ItemType& newEntry);
    bool remove(const ItemType& target);
    void clear();

    bool contains(const ItemType& anEntry) const;

    void preorderTraverse(void visit(ItemType&)) const;
    void inorderTraverse(void visit(ItemType&)) const;
    void postorderTraverse(void visit(ItemType&)) const;

    BinarySearchTree<ItemType>& operator=(const BinarySearchTree<ItemType>& rightHandSide);

};

//Constructors

//Default Ctor
template<class ItemType>
BinarySearchTree<ItemType>::BinarySearchTree() {
    rootPtr = nullptr;
}

//Ctor that defines rootItem
template<class ItemType>
BinarySearchTree<ItemType>::BinarySearchTree(const ItemType &rootItem) {
    rootPtr = new BinaryNode<ItemType>(rootItem);
}

//Copy Ctor
template<class ItemType>
BinarySearchTree<ItemType>::BinarySearchTree(const BinarySearchTree<ItemType> &tree) {
    rootPtr = copyTree(tree);
}

//Destructor
template<class ItemType>
BinarySearchTree<ItemType>::~BinarySearchTree() {
    this->destroyTree(rootPtr);
}

//Public Class Functions

//isEmpty(): returns true if the BST is empty, false if not
template<class ItemType>
bool BinarySearchTree<ItemType>::isEmpty() const {
    return (rootPtr == nullptr);
}

//getHeight(): returns the height of the BST
template<class ItemType>
int BinarySearchTree<ItemType>::getHeight() const {
    return this->getHeightHelper(rootPtr);
}

template<class ItemType>
int BinarySearchTree<ItemType>::getNumberOfNodes() const {
    return this->getNumberOfNodesHelper(rootPtr);
}

//setRootData(): sets the value of rootPtr
template<class ItemType>
void BinarySearchTree<ItemType>::setRootData(const ItemType &newData) {
    if(isEmpty())
        rootPtr = new BinaryNode<ItemType>(newData);
    else
        rootPtr->setItem(newData);
}

//add(): adds a new value to the BST
template<class ItemType>
bool BinarySearchTree<ItemType>::add(const ItemType &newEntry) {
    BinaryNode<ItemType>* temp = new BinaryNode<ItemType>(newEntry);
    rootPtr = balancedAdd( rootPtr, temp );
    return true;
}

//remove(): removes a value from the BST
template<class ItemType>
bool BinarySearchTree<ItemType>::remove(const ItemType &target) {
    bool removed = false;
    rootPtr = removeValue(rootPtr, target, removed);
    return true;
}

//clear(): deletes all BST nodes
template<class ItemType>
void BinarySearchTree<ItemType>::clear() {
    destroyTree(rootPtr);
    rootPtr = nullptr;
}

//contatins(): returns true if target value is contained within the BST
template<class ItemType>
bool BinarySearchTree<ItemType>::contains(const ItemType &anEntry) const {
    if(findNode(anEntry) == nullptr)
        return false;
    else
        return true;
}

//preorderTraverse(): traverse the BST in preorder
template<class ItemType>
void BinarySearchTree<ItemType>::preorderTraverse(void visit(ItemType &)) const {
    this->preorder(visit, rootPtr);
}

//inorderTraverse(): traverse the BST in inorder
template<class ItemType>
void BinarySearchTree<ItemType>::inorderTraverse(void visit(ItemType &)) const {
    this->inorder(visit, rootPtr);
}

//postorderTraverse(): traverse the BST in postorder
template<class ItemType>
void BinarySearchTree<ItemType>::postorderTraverse(void visit(ItemType &)) const {
    this->postorder(visit, rootPtr);
}

//assignment operator overload
template<class ItemType>
BinarySearchTree<ItemType>& BinarySearchTree<ItemType>::operator=(const BinarySearchTree<ItemType> &rightHandSide) {
    clear();
    rootPtr = copyTree(rightHandSide);
}

//Protected functions

//placeNode(): places a node at the given subTree
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::placeNode(BinaryNode<ItemType> *subTreePtr, BinaryNode<ItemType> *newNodePtr) {
    BinaryNode<ItemType>* tempPtr;
    if (subTreePtr == nullptr)
        return newNodePtr;
    else if (subTreePtr->getItem() > newNodePtr->getItem())
    {
        tempPtr = placeNode(subTreePtr->getLeftChildPtr(), newNodePtr);
        subTreePtr->setLeftChildPtr(tempPtr);
    }
    else
    {
        tempPtr = placeNode(subTreePtr->getRightChildPtr(), newNodePtr);
        subTreePtr->setRightChildPtr(tempPtr);
    }
    return subTreePtr;
}

//removeValue(): removes value of the target node
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::removeValue(BinaryNode<ItemType> *subTreePtr, const ItemType target,
                                                              bool &isSuccessful) {
    BinaryNode<ItemType>* temp = findNode(target);
    if(temp != nullptr){
        remove(target);
        return true;
    } else{
        return false;
    }
}

//removeNode(): removes target node
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::removeNode(BinaryNode<ItemType> *nodePtr) {
    BinaryNode<ItemType>* nodeToRemove;
    if(nodePtr->isLeaf()){
        delete nodePtr;
        nodePtr = nullptr;
        return nodePtr;
    } else if((nodePtr->getLeftChildPtr() != nullptr && nodePtr->getLeftChildPtr() == nullptr) || (nodePtr->getLeftChildPtr() == nullptr && nodePtr->getLeftChildPtr() != nullptr)){
        if(nodePtr->getLeftChildPtr() != nullptr){
            nodeToRemove = nodePtr->getLeftChildPtr();
        } else{
            nodeToRemove = nodePtr->getRightChildPtr();
        }
        return nodeToRemove;
    } else{
        ItemType newNodeValue;
        BinaryNode<ItemType>* temp = removeLeftmostNode(nodePtr->getRightChildPtr(), newNodeValue);
        nodePtr->setRightChildPtr(temp);
        nodePtr->setItem(newNodeValue);
        return nodePtr;
    }
}

//removeLeftmostNode(): removes the leftmost node of the BST
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::removeLeftmostNode(BinaryNode<ItemType> *nodePtr,
                                                                     ItemType &inorderSuccessor) {
    BinaryNode<ItemType>* tempPtr;
    if (nodePtr->getLeftChildPtr() == nullptr)
    {
// This is the node you want; it has no left child, but it might have a right subtree
        inorderSuccessor = nodePtr->getItem();
        return removeNode(nodePtr);
    }
    else
    {
        tempPtr = removeLeftmostNode(nodePtr->getLeftChildPtr(), inorderSuccessor);
        nodePtr->setLeftChildPtr(tempPtr);
        return nodePtr;
    }
}

//findNode(): returns a pointer to target node
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::findNode(BinaryNode<ItemType> *subTreePtr,
                                                           const ItemType &target) const {
    if (subTreePtr == nullptr)
        return nullptr; // Not found
    else if (subTreePtr->getItem() == target)
        return subTreePtr; // Found
    else if (subTreePtr->getItem() > target)
// Search left subtree
        return findNode(subTreePtr->getLeftChildPtr(), target);
    else
// Search right subtree
        return findNode(subTreePtr->getRightChildPtr(), target);
}

template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::balancedAdd(BinaryNode<ItemType> *subTreePtr,
                                                              BinaryNode<ItemType> *newNodePtr) {
    BinaryNode<ItemType>* tempPtr;

    if ( subTreePtr == nullptr )
        return newNodePtr;
    else if ( subTreePtr->getItem() > newNodePtr->getItem() )
    {
        tempPtr = balancedAdd(subTreePtr->getLeftChildPtr(), newNodePtr );
        subTreePtr->setLeftChildPtr(tempPtr);
    }
    else
    {
        tempPtr = balancedAdd(subTreePtr->getRightChildPtr(), newNodePtr );
        subTreePtr->setRightChildPtr(tempPtr);
    }
    return subTreePtr;
}