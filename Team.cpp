//
// Created by Toam Elharar on 26/11/2022.
//

#include "Team.h"

Team::Team():id(0),points(0),goal_keeper_status(false),num_of_games_played(0),num_of_players(0),
total_ability(0),total_spirit(permutation_t::neutral()) ,captain(nullptr){}

Team::Team(int id, int points, bool goal_keeper_status, int num_of_games_played, int num_of_players,
           int total_ability,permutation_t total_spirit, PlayerUFNode* captain):
           id(id), points(points), goal_keeper_status(goal_keeper_status), num_of_games_played(num_of_games_played),
           num_of_players(num_of_players), total_ability(total_ability),total_spirit(total_spirit), captain(captain){}

int Team::getId(){
    return this->id;
}

int Team::getPoints(){
    return this->points;
}

bool Team::getGoalKeeperStatus(){
    return this->goal_keeper_status;
}

int Team::getNumOfGamesPlayed(){
    return this->num_of_games_played;
}

int Team::getTotalAbility() {
    return this->total_ability;
}

permutation_t Team::getTotalSpirit() {
    return this->total_spirit;
}

PlayerUFNode* Team::getCaptain(){
    return this->captain;
}

void Team::setId(int id){
    this->id = id;
}

void Team::setPoints(int points){
    this->points = points;
}

void Team::setGoalKeeperStatus(bool status){
    this->goal_keeper_status = status;
}

void Team::setNumOfGamesPlayed(int num_of_games_played){
    this->num_of_games_played = num_of_games_played;
}

void Team::setTotalAbility(int ability) {
    this->total_ability = ability;
}

void Team::setTotalSpirit(const permutation_t spirit) {
    this->total_spirit = spirit;
}

void Team::setCaptain(PlayerUFNode* player){
    this->captain = player;
}

int Team::getNumOfPlayers() {
    return num_of_players;
}

void Team::setNumOfPlayers(int new_num_of_players) {
    num_of_players = new_num_of_players;
}