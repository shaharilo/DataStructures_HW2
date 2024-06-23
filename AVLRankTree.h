//
// Created by shaha on 29/11/2022.
//

#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H
#pragma once

#include <iostream>
#include "AVLRankTreeNode.h"
using std::cout;
using std::endl;

enum returnOption {SUCCESS,FAILURE,ALREADY_EXIST, NOT_VALID};
enum operationType {INSERT,REMOVE};

template<class Data>
class AVLRankTree {
    AVLRankTreeNode<Data> *root;
    int num_of_node;

public:
    AVLRankTree() : root(nullptr), num_of_node(0) {};

    virtual ~AVLRankTree() = default;

    AVLRankTreeNode<Data> *getRoot() {
        return this->root;
    }

    int getNumOfNode() {
        return this->num_of_node;
    }

    void setRoot(AVLRankTreeNode<Data> *new_root) {
        this->root = new_root;
    }

    void setNumOfNode(int new_num_of_node) {
        this->num_of_node = new_num_of_node;
    }

    AVLRankTreeNode<Data> *findNode(AVLRankTreeNode<Data> *node_to_find) {
        AVLRankTreeNode<Data> *current_root = this->root;
        while (current_root != nullptr) {
            if (*current_root == *node_to_find) {
                return current_root;
            } else if (*current_root > *node_to_find) {
                current_root = current_root->getLeft();
            } else {
                current_root = current_root->getRight();
            }
        }
        return nullptr;
    }

    AVLRankTreeNode<Data> *findNode(int key) {
        AVLRankTreeNode<Data> *current_root = this->root;
        while (current_root != nullptr) {
            if (current_root->getSortKey1() == key) {
                return current_root;
            } else if (current_root->getSortKey1() > key) {
                current_root = current_root->getLeft();
            } else {
                current_root = current_root->getRight();
            }
        }
        return nullptr;
    }

    returnOption insertNode(AVLRankTreeNode<Data> *new_node) {
        if (this->findNode(new_node) != nullptr) return ALREADY_EXIST;
        AVLRankTreeNode<Data> *current_root = this->root;
        if (!current_root) this->setRoot(new_node);
        while (current_root != nullptr) {
            if (*current_root > *new_node) {
                if (current_root->getLeft() == nullptr) {
                    current_root->setLeft(new_node);
                    new_node->setParent(current_root);
                    break;
                } else {
                    current_root = current_root->getLeft();
                }
            } else {
                if (current_root->getRight() == nullptr) {
                    current_root->setRight(new_node);
                    new_node->setParent(current_root);
                    break;
                } else {
                    current_root = current_root->getRight();
                }
            }
        }
        current_root = new_node;
        while (current_root) {
            updateHeightAndBalanceFactorForNode(current_root);
            if (current_root->getBalanceFactor() == 2 || current_root->getBalanceFactor() == -2) break;
            current_root = current_root->getParent();
        }
        fixAVLTreeBalance(new_node, INSERT);

        this->setNumOfNode((this->getNumOfNode()) + 1);
        return SUCCESS;
    }

    //ATTENTION:The function assumes that the root is one of the fatherhood of the leaf
    void updateAVLTreeHeightsAndBalanceFactorFromLeafToRoot(AVLRankTreeNode<Data> *leaf, AVLRankTreeNode<Data> *root) {
        if (!root || !leaf) return;

        AVLRankTreeNode<Data> *curr_node;
        if (leaf->nodeIsLeaf()) {
            leaf->setHeight(0);
            leaf->setBalanceFactor(0);
            leaf->setWeight(1);

            curr_node = leaf->getParent();
        } else curr_node = leaf;

        if (curr_node == nullptr) return;
        AVLRankTreeNode<Data> *curr_son = leaf;
        while (curr_node != root) {
            updateHeightAndBalanceFactorForNode(curr_node);
            curr_son = curr_node;
            curr_node = curr_node->getParent();
        }

        AVLRankTreeNode<Data> *root_before_rotate;
        side son_side = root->whichSideOfMeTheChildIs(curr_son);
        if (son_side == LEFT) root_before_rotate = root->getRight();
        else if (son_side == RIGHT) root_before_rotate = root->getLeft();

        if (root_before_rotate) updateHeightAndBalanceFactorForNode(root_before_rotate);
        updateHeightAndBalanceFactorForNode(root);



        while (root->getParent()) {
            updateHeightAndBalanceFactorForNode(root->getParent());
            root = root->getParent();
        }
    }

    void updateHeightAndBalanceFactorForNode(AVLRankTreeNode<Data> *node) {
        AVLRankTreeNode<Data> *left = node->getLeft();
        AVLRankTreeNode<Data> *right = node->getRight();
        if (left == nullptr && right == nullptr) {
            node->setHeight(0);
            node->setBalanceFactor(0);
            node->setWeight(1);

            return;
        }

        //update Heights
        if (!left) node->setHeight(right->getHeight() + 1);
        else if (!right) node->setHeight(left->getHeight() + 1);
        else {
            int left_height = left->getHeight();
            int right_height = right->getHeight();
            int max_height = right_height;
            if (left_height > right_height) max_height = left_height;
            node->setHeight(max_height + 1);
        }

        //update Balance Factor
        if (!left) node->setBalanceFactor(-1 - right->getHeight());
        else if (!right) node->setBalanceFactor(left->getHeight() +1);
        else node->setBalanceFactor(left->getHeight() - right->getHeight());

        //update wight
        if (!left) node->setWeight(right->getWeight() + 1);
        else if (!right) node->setWeight(left->getWeight() + 1);
        else node->setWeight(right->getWeight() + left->getWeight() + 1);
    }

    void updateHeightAndBalanceAfterUnite(AVLRankTreeNode<Data> *sub_root) {
        if (!sub_root) return;

        updateHeightAndBalanceAfterUnite(sub_root->getLeft());
        updateHeightAndBalanceAfterUnite(sub_root->getRight());
        this->updateHeightAndBalanceFactorForNode(sub_root);
    }

    //here need to add recursion because when you delete node you have to do the fixing up until the root
    //bf = BalanceFactor
    void fixAVLTreeBalance(AVLRankTreeNode<Data>* inserted_node, operationType operation_type) {
        if (inserted_node == nullptr) return;
        //AVLRankTreeNode<Data> *curr_node = inserted_node->getParent();
        AVLRankTreeNode<Data> *curr_node = inserted_node;


        int left_bf = 0, right_bf = 0;

        if(curr_node== nullptr) return;
        int node_bf = curr_node->getBalanceFactor();
        while (curr_node) {
            if (node_bf == 2 || node_bf == -2) {
                if (curr_node->getRight() != nullptr) {
                    right_bf = curr_node->getRight()->getBalanceFactor();
                }
                if (curr_node->getLeft() != nullptr) {
                    left_bf = curr_node->getLeft()->getBalanceFactor();
                }

                if (node_bf == 2) {
                    if (left_bf >= 0) {
                        if (operation_type == INSERT) {
                            this->rotateLL(curr_node, inserted_node);
                            return;
                        }
                        else{
                            this->rotateLL(curr_node, curr_node->getLeft()->getLeft());
                        }
                    } else if (left_bf == -1) {
                        if (operation_type == INSERT) {
                            this->rotateLR(curr_node, inserted_node);
                            return;
                        }
                        else{
                            this->rotateLR(curr_node, curr_node->getLeft()->getRight());
                        }
                    }
                } else {
                    if (right_bf <= 0) {
                        if (operation_type == INSERT) {
                            this->rotateRR(curr_node, inserted_node);
                            return;
                        }
                        else{
                            this->rotateRR(curr_node, curr_node->getRight()->getRight());
                        }
                    } else if (right_bf == 1) {
                        if (operation_type == INSERT) {
                            this->rotateRL(curr_node, inserted_node);
                            return;
                        }
                        else{
                            this->rotateRL(curr_node, curr_node->getRight()->getLeft());
                        }
                    }
                }
            }
            curr_node = curr_node->getParent();
            if (curr_node) node_bf = curr_node->getBalanceFactor();
            left_bf = 0;
            right_bf = 0;
        }
    }

    returnOption removeNode(AVLRankTreeNode<Data> *node_to_delete, removeNodeOption remove_node_option = WITHOUT_RELEASING_DATA) {
        if (!findNode(node_to_delete)) return NOT_VALID;

        if (node_to_delete->nodeIsLeaf() && node_to_delete->getParent() == nullptr){ //the last node in the AVLRankTree
            if(remove_node_option == RELEASE_DATA) delete node_to_delete->getData();
            delete node_to_delete;
            this->setNumOfNode((this->getNumOfNode()) - 1);
            this->setRoot(nullptr);
            return SUCCESS;
        }

        side side_of_node_to_delete = node_to_delete->whichSideOfTheFatherPointsToMe();

        if (node_to_delete->nodeIsLeaf()){
            AVLRankTreeNode<Data>* parent = node_to_delete->getParent();
            if (side_of_node_to_delete == LEFT) parent->setLeft(nullptr);
            else if (side_of_node_to_delete == RIGHT) parent->setRight(nullptr);
            else this->setRoot(nullptr);

            if (remove_node_option == RELEASE_DATA)  delete node_to_delete->getData();
            setNodePointersNULL(node_to_delete, remove_node_option);
            delete node_to_delete;

            AVLRankTreeNode<Data>* current_root = parent;
            while(current_root){
                updateHeightAndBalanceFactorForNode(current_root);
                current_root = current_root->getParent();
            }
            fixAVLTreeBalance(parent, REMOVE);

            this->setNumOfNode((this->getNumOfNode()) - 1);
            return SUCCESS;
        }

        //node is not a leaf
        AVLRankTreeNode<Data> *new_root_of_sub_AVLTree;
        if(!node_to_delete->getRight()){//node that is erased has no right son
            new_root_of_sub_AVLTree = node_to_delete->getLeft();

            AVLRankTreeNode<Data>* parent = node_to_delete->getParent();
            if (side_of_node_to_delete == LEFT) parent->setLeft(new_root_of_sub_AVLTree);
            else if (side_of_node_to_delete == RIGHT) parent->setRight(new_root_of_sub_AVLTree);
            else this->setRoot(new_root_of_sub_AVLTree);

            new_root_of_sub_AVLTree->setParent(parent);
            if (remove_node_option == RELEASE_DATA) delete node_to_delete->getData();
            setNodePointersNULL(node_to_delete, remove_node_option);
            delete node_to_delete;

            AVLRankTreeNode<Data>* current_root = new_root_of_sub_AVLTree;
            while(current_root){
                updateHeightAndBalanceFactorForNode(current_root);
                current_root = current_root->getParent();
            }

            fixAVLTreeBalance(new_root_of_sub_AVLTree, REMOVE);
        }
        else {
            if ((node_to_delete->getRight())->getLeft()) {
                new_root_of_sub_AVLTree = findMinimumNodeAVLTree(node_to_delete->getRight());
                AVLRankTreeNode<Data> *new_root_of_sub_AVLTree_original_parent = new_root_of_sub_AVLTree->getParent();

                AVLRankTreeNode<Data> *parent = node_to_delete->getParent();
                if (side_of_node_to_delete == LEFT) {
                    parent->setLeft(new_root_of_sub_AVLTree);
                    new_root_of_sub_AVLTree->setParent(parent);
                }
                else if (side_of_node_to_delete == RIGHT) {
                    parent->setRight(new_root_of_sub_AVLTree);
                    new_root_of_sub_AVLTree->setParent(parent);
                }
                else {
                    this->setRoot(new_root_of_sub_AVLTree);
                    new_root_of_sub_AVLTree->setParent(nullptr);
                }

                new_root_of_sub_AVLTree_original_parent->setLeft(new_root_of_sub_AVLTree->getRight());
                if (new_root_of_sub_AVLTree_original_parent->getLeft())
                    (new_root_of_sub_AVLTree_original_parent->getLeft())->setParent(new_root_of_sub_AVLTree_original_parent);

                new_root_of_sub_AVLTree->setLeft(node_to_delete->getLeft());
                if (node_to_delete->getLeft()) node_to_delete->getLeft()->setParent(new_root_of_sub_AVLTree);

                new_root_of_sub_AVLTree->setRight(node_to_delete->getRight());
                if (node_to_delete->getRight()) node_to_delete->getRight()->setParent(new_root_of_sub_AVLTree);

                if (remove_node_option == RELEASE_DATA) delete node_to_delete->getData();
                setNodePointersNULL(node_to_delete, remove_node_option);
                delete node_to_delete;

                AVLRankTreeNode<Data>* current_root = new_root_of_sub_AVLTree_original_parent;
                while(current_root){
                    updateHeightAndBalanceFactorForNode(current_root);
                    current_root = current_root->getParent();
                }

                fixAVLTreeBalance(new_root_of_sub_AVLTree_original_parent, REMOVE);
            }
            else{
                new_root_of_sub_AVLTree = node_to_delete->getRight();
                AVLRankTreeNode<Data> *parent = node_to_delete->getParent();
                if (side_of_node_to_delete == LEFT) {
                    parent->setLeft(new_root_of_sub_AVLTree);
                    new_root_of_sub_AVLTree->setParent(parent);
                }
                else if (side_of_node_to_delete == RIGHT) {
                    parent->setRight(new_root_of_sub_AVLTree);
                    new_root_of_sub_AVLTree->setParent(parent);
                }
                else {
                    this->setRoot(new_root_of_sub_AVLTree);
                    new_root_of_sub_AVLTree->setParent(nullptr);
                }
                new_root_of_sub_AVLTree->setLeft(node_to_delete->getLeft());
                if (new_root_of_sub_AVLTree->getLeft()) (new_root_of_sub_AVLTree->getLeft())->setParent(new_root_of_sub_AVLTree);

                if (remove_node_option == RELEASE_DATA)  delete node_to_delete->getData();
                setNodePointersNULL(node_to_delete, remove_node_option);
                delete node_to_delete;

                AVLRankTreeNode<Data>* current_root = new_root_of_sub_AVLTree;
                while(current_root){
                    updateHeightAndBalanceFactorForNode(current_root);
                    current_root = current_root->getParent();
                }
                fixAVLTreeBalance(new_root_of_sub_AVLTree, REMOVE);
            }
        }

        this->setNumOfNode(this->getNumOfNode() - 1);
        return SUCCESS;
    }

    void rotateLL(AVLRankTreeNode<Data> *original_parent, AVLRankTreeNode<Data> *inserted_leaf){
        AVLRankTreeNode<Data> *Left_child = original_parent->getLeft();

        //Updating the left part of the right child to be the right child of the original parent
        AVLRankTreeNode<Data> *right_child_of_Left_child = Left_child->getRight();
        original_parent->setLeft(right_child_of_Left_child);
        if (right_child_of_Left_child) right_child_of_Left_child->setParent(original_parent);

        //Updating the pointer to this AVLRankTree, to be a pointer to the right part of the AVLRankTree
        side side_of_parent = original_parent->whichSideOfTheFatherPointsToMe();
        AVLRankTreeNode<Data> *pointer_to_parent = original_parent->getParent();
        if (side_of_parent == IS_ROOT) {
            this->setRoot(Left_child);
            Left_child->setParent(nullptr);
        }
        else {
            if (side_of_parent == RIGHT) pointer_to_parent->setRight(Left_child);
            else pointer_to_parent->setLeft(Left_child);

            Left_child->setParent(pointer_to_parent);
        }

        //Update the right child to be the parent of the original parent
        Left_child->setRight(original_parent);
        original_parent->setParent(Left_child);

        if (inserted_leaf == Left_child) {
            updateAVLTreeHeightsAndBalanceFactorFromLeafToRoot(original_parent, Left_child);
        }
        else updateAVLTreeHeightsAndBalanceFactorFromLeafToRoot(inserted_leaf, Left_child);

    }

    void rotateLR(AVLRankTreeNode<Data> *original_parent, AVLRankTreeNode<Data> *inserted_leaf) {
        this->rotateRR(original_parent->getLeft(),inserted_leaf);
        this->rotateLL(original_parent,inserted_leaf);

    }

    void rotateRR(AVLRankTreeNode<Data> *original_parent, AVLRankTreeNode<Data> *inserted_leaf) {
        AVLRankTreeNode<Data> *right_child = original_parent->getRight();

        //Updating the left part of the right child to be the right child of the original parent
        AVLRankTreeNode<Data> *left_child_of_right_child = right_child->getLeft();
        original_parent->setRight(left_child_of_right_child);
        if (left_child_of_right_child) left_child_of_right_child->setParent(original_parent);

        //Updating the pointer to this AVLRankTree, to be a pointer to the right part of the AVLRankTree
        side side_of_parent = original_parent->whichSideOfTheFatherPointsToMe();
        AVLRankTreeNode<Data> *pointer_to_parent = original_parent->getParent();
        if (side_of_parent == IS_ROOT) {
            this->setRoot(right_child);
            right_child->setParent(nullptr);
        }
        else {
            if (side_of_parent == RIGHT) pointer_to_parent->setRight(right_child);
            else pointer_to_parent->setLeft(right_child);

            right_child->setParent(pointer_to_parent);
        }

        //Update the right child to be the parent of the original parent
        right_child->setLeft(original_parent);
        original_parent->setParent(right_child);

        if (inserted_leaf == right_child) updateAVLTreeHeightsAndBalanceFactorFromLeafToRoot(original_parent, right_child);
        else updateAVLTreeHeightsAndBalanceFactorFromLeafToRoot(inserted_leaf, right_child);



    }

    void rotateRL(AVLRankTreeNode<Data> *original_parent, AVLRankTreeNode<Data> *inserted_leaf) {
        this->rotateLL(original_parent->getRight(),inserted_leaf);
        this->rotateRR(original_parent,inserted_leaf);
    }


    void removeAllNodesFromAVLTreeForUnite(AVLRankTreeNode<Data>* node){
        if (!node) return;
        if (node->nodeIsLeaf()) {
            if (node->getParent() != nullptr) {
                side me_to_parent_side = node->whichSideOfTheFatherPointsToMe();
                if (me_to_parent_side == RIGHT) node->getParent()->setRight(nullptr);
                else if (me_to_parent_side == LEFT) node->getParent()->setLeft(nullptr);
            }
            setNodePointersNULL(node);
            delete node;
            return;
        }
        removeAllNodesFromAVLTreeForUnite(node->getLeft());
        removeAllNodesFromAVLTreeForUnite(node->getRight());
        removeAllNodesFromAVLTreeForUnite(node);
    }

    AVLRankTreeNode<Data>* select(int index){
        AVLRankTreeNode<Data> *current_root = this->root;
        if (this->getNumOfNode() < index || index < 0) return nullptr;

        while (current_root != nullptr) {
            int weight_left;
            if (current_root->getLeft()) weight_left = current_root->getLeft()->getWeight();
            else weight_left = 0;

            if (weight_left > index - 1) {
                current_root = current_root->getLeft();
            }

            else if (weight_left == index - 1) {
                return current_root;
            }
            else {
                index = index - 1 - weight_left;
                current_root = current_root->getRight();
            }
        }
        return nullptr;
    }
};

template<class Data>
AVLRankTreeNode<Data>* findMinimumNodeAVLTree(AVLRankTreeNode<Data>* root){
    if (root->nodeIsLeaf()) return root;
    if (root->getLeft() != nullptr) return findMinimumNodeAVLTree(root->getLeft());
    else return root;
}

//
//#define COUNT 10
//template<class Data>
//void print2DUtil(AVLRankTreeNode<Data>* root, int space)
//{
//    // Base case
//    if (root == nullptr)
//        return;
//    // Increase distance between levels
//    space += COUNT;
//    // Process right child first
//    print2DUtil(root->getRight(), space);
//    // Print current node after space
//    // count
//    cout << endl;
//    for (int i = COUNT; i < space; i++)
//        cout << " ";
//    cout << root->getSortKey1()  << ", id:" << root->getData()->getId() <<", Rank:" << root->getWeight() << ", BF: " << root->getBalanceFactor() << ", H: " << root->getHeight() << "\n";
//    // Process left child
//    print2DUtil(root->getLeft(), space);
//}
//template<class Data>
//// Wrapper over print2DUtil()
//void print2D(AVLRankTreeNode<Data>* root)
//{
//    // Pass initial space count as 0
//
//    cout << endl << endl;
//    cout << "print of AVLRankTree" << endl;
//    print2DUtil(root, 0);
//}

#endif //WORLDCUP23A1_H_AVLTREE_H
