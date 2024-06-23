//
// Created by Toam Elharar on 26/11/2022.
//

#include "Player.h"

Player::Player():id(0), num_of_games_played(0), cards(0), goal_keeper_status(false), ability(0){
    partial_spirit = *(new permutation_t());
}

Player::Player(int id, int num_of_games_played,int cards,bool goal_keeper_status,
              const permutation_t partial_spirit, int ability):
               id(id), num_of_games_played(num_of_games_played), cards(cards),
               goal_keeper_status(goal_keeper_status), partial_spirit(partial_spirit), ability(ability) {}


int Player::getId() {
    return this->id;
}

int Player::getNumOfGamesPlayed(){
    return  this->num_of_games_played;
}

int Player::getCards(){
    return this->cards;
}
bool Player::getGoalKeeperStatus(){
    return this->goal_keeper_status;
}

permutation_t Player::getPartialSpirit(){
    return this->partial_spirit;
}

int Player::getAbility(){
    return this->ability;
}

void  Player::setNumOfGamesPlayed(int num){
    this->num_of_games_played = num;
}

void  Player::setCards(int new_cards){
    this->cards = new_cards;
}

void  Player::setGoalKeeperStatus(bool status){
    this->goal_keeper_status = status;
}

void Player::setPartialSpirit(permutation_t new_spirit){
    this->partial_spirit = new_spirit;
}

void Player::setAbility(int new_ability){
    this->ability = new_ability;
}
