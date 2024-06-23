//
// Created by shaha on 29/11/2022.
//

#ifndef WET1_AVLTREENODE_H
#define WET1_AVLTREENODE_H
#pragma once


enum side {RIGHT,LEFT,IS_ROOT, IS_NOT_A_CHILD};
enum removeNodeOption {RELEASE_DATA, WITHOUT_RELEASING_DATA};

//DONT INCLUDE ANYTHING HERE!!!!!!!!! WE DONT WANT CIRCLES

template<class Data>
class AVLRankTreeNode{
    Data data;
    int sort_key1;
    int sort_key2;
    AVLRankTreeNode* left;
    AVLRankTreeNode* right;
    AVLRankTreeNode* parent;
    int height;
    int balance_factor;
    int weight;

public:
    AVLRankTreeNode(): data(nullptr), sort_key1(0), sort_key2(0), left(nullptr), right(nullptr), parent(nullptr),
    height(0), balance_factor(0), weight(0){};
    explicit AVLRankTreeNode(Data data, int sort_key1, int sort_key2 = 0, AVLRankTreeNode* left = nullptr,
                             AVLRankTreeNode* right = nullptr, AVLRankTreeNode* parent = nullptr , int height = 0,
                             int balance_factor = 0, int weight = 0):
            data(data),sort_key1(sort_key1),sort_key2(sort_key2),left(left),right(right),parent(parent),height(height),
            balance_factor(balance_factor), weight(weight){};
    ~AVLRankTreeNode() = default;
    AVLRankTreeNode(const AVLRankTreeNode &tree_node) = default;

    Data getData(){
        return this->data;
    }

    AVLRankTreeNode* getRight() {
        return this->right;
    }

    AVLRankTreeNode* getLeft() {
        return this->left;
    }

    AVLRankTreeNode* getParent() {
        return this->parent;
    }

    int getSortKey1(){
        return this->sort_key1;
    }

    int getSortKey2(){
        return this->sort_key2;
    }

    int getHeight(){
        return this->height;
    }

    int getBalanceFactor() {
        return this->balance_factor;
    }

    int getWeight() {
        return this->weight;
    }


    void setData(Data data){
        this->data = data;
    }

    void setRight(AVLRankTreeNode* node) {
        this->right = node;
    }

    void setLeft(AVLRankTreeNode* node) {
        this->left = node;
    }

    void setParent(AVLRankTreeNode* node) {
        this->parent = node;
    }

    void setSortKey1(int key){
        this->sort_key1 = key;
    }

    void setSortKey2(int key){
        this->sort_key2 = key;
    }

    void setKey(int key1, int key2 = 0) {
        this->setSortKey1(key1);
        this->setSortKey2(key2);
    }

    void setHeight(int height){
        this->height = height;
    }

    void addHeight(int height_to_add){
        this->height += height_to_add;
    }

    void setBalanceFactor(int bf) {
        this->balance_factor = bf;
    }

    void setWeight(int weight){
        this->weight = weight;
    }

    side whichSideOfTheFatherPointsToMe(){
        AVLRankTreeNode *pointer_to_parent = this->getParent();
        if(!pointer_to_parent) return IS_ROOT;
        if (pointer_to_parent->getLeft() == this) return LEFT;
        return RIGHT;
    }

    side whichSideOfMeTheChildIs(AVLRankTreeNode* child){
        if(this->left == child) return LEFT;
        else if(this->right == child) return RIGHT;
        return IS_NOT_A_CHILD;
    }

    bool nodeIsLeaf(){
        AVLRankTreeNode* left_son = this->left;
        AVLRankTreeNode* right_son = this->right;
        if (left_son== nullptr && right_son== nullptr) return true;
        return false;
    }

    friend bool operator>(AVLRankTreeNode& node1, AVLRankTreeNode& node2) {
        if (node1.getSortKey1() > node2.getSortKey1()) return true;
        else if (node1.getSortKey1() < node2.getSortKey1()) return false;
        //sort_key1 is the same

        if (node1.getSortKey2() > node2.getSortKey2()) return true;
        else if (node1.getSortKey2() < node2.getSortKey2()) return false;
        //sort_key2 is the same

        return false;

    }

    friend bool operator<(AVLRankTreeNode& node1, AVLRankTreeNode& node2){
        if (node1.getSortKey1() > node2.getSortKey1()) return false;
        else if (node1.getSortKey1() < node2.getSortKey1()) return true;
        //sort_key1 is the same

        if (node1.getSortKey2() > node2.getSortKey2()) return false;
        else if (node1.getSortKey2() < node2.getSortKey2()) return true;
        //sort_key2 is the same

        return false;
    }



};

template<class Data>
bool operator==(AVLRankTreeNode<Data>& node1, AVLRankTreeNode<Data>& node2){
    return !(node1 > node2 || node1 < node2);
}

template<class Data>
void setNodePointersNULL(AVLRankTreeNode<Data>* node, removeNodeOption remove_node_option = WITHOUT_RELEASING_DATA){
    if (remove_node_option == WITHOUT_RELEASING_DATA) node->setData(nullptr);
    node->setLeft(nullptr);
    node->setRight(nullptr);
    node->setParent(nullptr);
}

#endif //WET1_AVLTREENODE_H
