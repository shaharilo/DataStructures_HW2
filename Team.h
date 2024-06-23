//
// Created by Toam Elharar on 26/11/2022.
//

#ifndef WET1_TEAM_H
#define WET1_TEAM_H
#pragma once

#include "wet2util.h"
#include "PlayerUFNode.h"
#pragma once

class PlayerUFNode;

class Team {
private:
    int id;
    int points;
    bool goal_keeper_status;
    int num_of_games_played;
    int num_of_players;
    int total_ability;
    permutation_t total_spirit;
    PlayerUFNode* captain;


public:
    Team();
    explicit Team(int id, int points = 0 , bool goal_keeper_status = false, int num_of_games_played = 0,
                  int num_of_players = 0, int total_ability = 0, permutation_t total_spirit = permutation_t::neutral(), PlayerUFNode* captain = nullptr);
    virtual ~Team() = default;
    Team(const Team& team) = default;

    int getId();
    int getPoints();
    bool getGoalKeeperStatus();
    int getNumOfGamesPlayed();
    int getNumOfPlayers();
    int getTotalAbility();
    permutation_t getTotalSpirit();
    PlayerUFNode* getCaptain();

    void setId(int id);
    void setPoints(int points);
    void setGoalKeeperStatus(bool status);
    void setNumOfGamesPlayed(int num_of_games_played);
    void setNumOfPlayers(int new_num_of_players);
    void setTotalAbility(int ability);
    void setTotalSpirit(const permutation_t spirit);
    void setCaptain(PlayerUFNode* player);
};

#endif //WORLDCUP23A1_H_TEAM_H
