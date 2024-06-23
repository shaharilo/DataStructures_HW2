//
// Created by Toam Elharar on 26/11/2022.
//

#ifndef WET1_PLAYER_H
#define WET1_PLAYER_H
#pragma once

#include "wet2util.h"
class Team;


class Player {
    int id;
    int num_of_games_played;
    int cards;
    bool goal_keeper_status;
    permutation_t partial_spirit;
    int ability;

public:
    Player();
    explicit Player(int id, int num_of_games_played,int cards,bool goal_keeper_status,
                     permutation_t partial_spirit, int ability);
    virtual ~Player() = default;
    Player(const Player& player) = default;

    int getId();
    int getNumOfGamesPlayed();
    int getCards();
    bool getGoalKeeperStatus();
    permutation_t getPartialSpirit();
    int getAbility();

    void setNumOfGamesPlayed(int num);
    void setCards(int cards);
    void setGoalKeeperStatus(bool status);
    void setPartialSpirit(permutation_t spirit);
    void setAbility(int ability);
};

#endif // WORLDCUP23A1_H_PLAYER_H
