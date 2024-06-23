//
// Created by Toam Elharar on 26/11/2022.
//

#ifndef WET1_TEAMSTREE_H
#define WET1_TEAMSTREE_H
#pragma once

#include "Team.h"
#include "AVLRankTree.h"

class TeamsAVLRankTree : public AVLRankTree<Team*>{
public:
    TeamsAVLRankTree(): AVLRankTree<Team*>(){};
    virtual ~TeamsAVLRankTree() = default;

    void removeAllNodesFromTree(AVLRankTreeNode<Team*>* root, removeNodeOption remove_node_option = WITHOUT_RELEASING_DATA){
        if (!root) return;
        if (root->nodeIsLeaf()){

            side me_to_parent_side = root->whichSideOfTheFatherPointsToMe();
            if (me_to_parent_side == RIGHT) root->getParent()->setRight(nullptr);
            else if (me_to_parent_side == LEFT) root->getParent()->setLeft(nullptr);
            else this->setRoot(nullptr);

            if(remove_node_option == RELEASE_DATA){
                delete (root->getData());
            }

            setNodePointersNULL(root);
            delete root;
            return;
        }
        removeAllNodesFromTree(root->getRight(), remove_node_option);
        removeAllNodesFromTree(root->getLeft(), remove_node_option);
        removeAllNodesFromTree(root, remove_node_option);
    }
};

#endif //WET1_teamsTree_H
