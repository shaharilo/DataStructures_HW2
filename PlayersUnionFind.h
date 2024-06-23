
#ifndef DATA_STRUCTURES_WET2_PLAYERSUNIONFIND_H
#define DATA_STRUCTURES_WET2_PLAYERSUNIONFIND_H
#pragma once

#include <iostream>
#include "PlayersHashTable.h"
#include "PlayerUFNode.h"
#include "ListForHash.h"

using std::cout;
using std::endl;

class PlayersUnionFind{
private:
    PlayersHashTable* hash_table;
    int num_of_players;

public:
    PlayersUnionFind();
    ~PlayersUnionFind();

    PlayersHashTable* getHashTable();
    int getNumOfPlayers();

    void setHashTable(PlayersHashTable* hash_table);
    void setNumOfPlayers(int num);

    void addNode(PlayerUFNode* new_player_node);
    void unionTeams(Team* buyerTeam, Team* boughtTeam);
    PlayerUFNode* findRoot(int key); //find of union find with shrinking tracks

    int gamesFromNodeToRoot(PlayerUFNode* node);
    permutation_t spiritFromNodeToRoot(PlayerUFNode* node);
    //void printUF();
};

#endif //DATA_STRUCTURES_WET2_PLAYERSUNIONFIND_H
