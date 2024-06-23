
#include "PlayerUFNode.h"
#define INIT_PARENT_VALUE -1


PlayerUFNode::PlayerUFNode(): players_team(nullptr), player(nullptr),parent_id(INIT_PARENT_VALUE){}
PlayerUFNode::PlayerUFNode(Team* team, Player* player, int parent_id):
players_team(team), player(player),parent_id(parent_id){}

Team* PlayerUFNode::getTeam(){
    return this->players_team;
}

int PlayerUFNode::getParentId(){
    return this->parent_id;
}

Player* PlayerUFNode::getPlayer(){
    return this->player;
}

void PlayerUFNode::setTeam(Team* team_){
    this->players_team = team_;
}

void PlayerUFNode::setParentId(int parent_id_){
    this->parent_id = parent_id_;
}

void PlayerUFNode::setPlayer(Player* player_){
    this->player = player_;
}
