//
// Created by Toam Elharar on 21/12/2022.
//

#ifndef DATA_STRUCTURES_WET2_PLAYERUFNODE_H
#define DATA_STRUCTURES_WET2_PLAYERUFNODE_H
#pragma once

#include "Team.h"
#include "Player.h"

class PlayerUFNode {
    Team* players_team;
    Player* player;
    int parent_id;

public:
    PlayerUFNode();
    explicit PlayerUFNode(Team* team, Player* player, int parent_id = -1);
    virtual ~PlayerUFNode() = default;

    Team* getTeam();
    int getParentId();
    Player* getPlayer();

    void setTeam(Team* team_);
    void setParentId(int parent_id);
    void setPlayer(Player* player_);
};


#endif //DATA_STRUCTURES_WET2_PLAYERUFNODE_H
