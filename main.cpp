//
// Created by Raymond Heinzelman on 2019-03-24.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "BinarySearchTree.h"

using namespace std;

template <class ItemType>
void visitHelper(ItemType& val);

int main(){

    srand(time(NULL));

    //BST Declaration
    BinarySearchTree<int> BST;

    int randNums[100];

    //unique number generator
    int temp;
    for(int i = 0; i < 100; i++){
        bool repeat = true;
        while(repeat){
            repeat = false;
            temp = rand() % 201;
            for(int j = 0; j < i-1; j++){
                if(temp == randNums[j]){
                    repeat = true;
                }
            }
        }//end while()
        randNums[i] = temp;
    }

    for(int i = 0; i < 100; i++){
        BST.add(randNums[i]);
    }

    cout << endl;

    cout << "Binary Search Tree filled with 100 random integers ranging from 0-200" << endl << endl;

    cout << "Height: " << BST.getHeight() << endl << endl;

    cout << "Preorder Traversal: " << endl;
    BST.preorderTraverse(visitHelper);
    cout << endl << endl;

    cout << "Inorder Traversal: " << endl;
    BST.inorderTraverse(visitHelper);
    cout << endl << endl;

    cout << "Postorder Traversal: " << endl;
    BST.postorderTraverse(visitHelper);
    cout << endl << endl;

    return 0;
}

template <class ItemType>
void visitHelper(ItemType& val){
    cout << val << ", ";
}
