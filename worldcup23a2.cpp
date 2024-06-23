#include "worldcup23a2.h"

world_cup_t::world_cup_t() {
    teams_tree_by_id = new TeamsAVLRankTree();
    teams_tree_by_ability = new TeamsAVLRankTree();
    players_union_find = new PlayersUnionFind();
}

world_cup_t::~world_cup_t()
{
    teams_tree_by_ability->removeAllNodesFromTree(teams_tree_by_ability->getRoot());
    delete teams_tree_by_ability;

    teams_tree_by_id->removeAllNodesFromTree(teams_tree_by_id->getRoot(),RELEASE_DATA);
    delete teams_tree_by_id;

    delete players_union_find;
}

StatusType world_cup_t::add_team(int teamId)
{
    if(teamId<=0) return StatusType::INVALID_INPUT;
    if(teams_tree_by_id->findNode(teamId)!= nullptr) return StatusType::FAILURE;

    Team* new_team = new (std::nothrow)Team(teamId);
    if(new_team== nullptr) return StatusType::ALLOCATION_ERROR;

    AVLRankTreeNode<Team*>* team_node_by_id = new (std::nothrow)AVLRankTreeNode<Team*>(new_team,teamId);
    if(team_node_by_id== nullptr){
        delete new_team;
        return StatusType::ALLOCATION_ERROR;
    }

    int initial_ability = 0;
    AVLRankTreeNode<Team*>* team_node_by_ability = new (std::nothrow)AVLRankTreeNode<Team*>(new_team,initial_ability,teamId);
    if(team_node_by_ability== nullptr){
        delete team_node_by_id;
        delete new_team;
        return StatusType::ALLOCATION_ERROR;
    }

    teams_tree_by_id->insertNode(team_node_by_id);
    teams_tree_by_ability->insertNode(team_node_by_ability);
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if(teamId<=0) return StatusType::INVALID_INPUT;
    AVLRankTreeNode<Team*>* team_node_by_id = teams_tree_by_id->findNode(teamId);
    if(team_node_by_id == nullptr) return StatusType::FAILURE;
    Team* team_to_remove = team_node_by_id->getData();

    AVLRankTreeNode<Team*> temp1;
    temp1.setSortKey1(team_to_remove->getTotalAbility());
    temp1.setSortKey2(teamId);
    AVLRankTreeNode<Team*>* team_node_by_ability = teams_tree_by_ability->findNode(&temp1);

    int num_of_players = team_to_remove->getNumOfPlayers();
    if(num_of_players != 0){
        //there are players in team:
        Player* captain = team_to_remove->getCaptain()->getPlayer();
        captain->setNumOfGamesPlayed(captain->getNumOfGamesPlayed() + team_to_remove->getNumOfGamesPlayed());
        PlayerUFNode* captain_node = team_to_remove->getCaptain();
        captain_node->setTeam(nullptr);
    }
    teams_tree_by_ability->removeNode(team_node_by_ability);
    teams_tree_by_id->removeNode(team_node_by_id,RELEASE_DATA);
    return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId,const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper) {
    if(teamId<=0 || playerId <=0 || gamesPlayed <0 || cards < 0 || !(spirit.isvalid())) return StatusType::INVALID_INPUT;
    if(players_union_find->getHashTable()->findElement(playerId) != nullptr) return StatusType::FAILURE;
    if(teams_tree_by_id->findNode(teamId) == nullptr) return StatusType::FAILURE;

    Team* team_of_player = teams_tree_by_id->findNode(teamId)->getData();

    PlayerUFNode* root = team_of_player->getCaptain();
    permutation_t partial_spirit;
    if (root == nullptr) {
        team_of_player->setTotalSpirit(spirit);
        partial_spirit = spirit;
    }
    else {
        team_of_player->setTotalSpirit((team_of_player->getTotalSpirit()) * spirit);
        partial_spirit = ((root->getPlayer()->getPartialSpirit()).inv() * team_of_player->getTotalSpirit());
    }


    Player* player_to_add = new (std::nothrow) Player(playerId, gamesPlayed, cards, goalKeeper, partial_spirit, ability);
    if(player_to_add == nullptr) {
        return StatusType::ALLOCATION_ERROR;
    }


    AVLRankTreeNode<Team*> temp;
    temp.setSortKey1(team_of_player->getTotalAbility());
    temp.setSortKey2(teamId);

    //remove and insert the team's node in the ability tree - to maintain the balance of the AVL tree
    AVLRankTreeNode<Team*>* team_by_ability = teams_tree_by_ability->findNode(&temp);
    teams_tree_by_ability->removeNode(team_by_ability);


    //update the team's properties
    int team_ability = team_of_player->getTotalAbility();
    team_of_player->setTotalAbility(team_ability + ability);
    AVLRankTreeNode<Team*>* team_node_by_ability = new (std::nothrow)AVLRankTreeNode<Team*>(team_of_player,team_of_player->getTotalAbility(),teamId);
    if(team_node_by_ability== nullptr){
        delete player_to_add;
        return StatusType::ALLOCATION_ERROR;
    }
    teams_tree_by_ability->insertNode(team_node_by_ability);


    //add the player to the union find
    PlayerUFNode* new_player_node;

    if(team_of_player->getNumOfPlayers() == 0){
        //meaning this player will be the captain
        new_player_node = new (std::nothrow)PlayerUFNode(team_of_player,player_to_add);
        if(new_player_node == nullptr){
            delete team_node_by_ability;
            delete player_to_add;
            return StatusType::ALLOCATION_ERROR;
        }
        team_of_player->setCaptain(new_player_node);
    }
    else{
        //there are other players in this team - team will be null, if we want to know the player's team - need to go to the captain
        new_player_node = new  (std::nothrow)PlayerUFNode(nullptr,player_to_add, team_of_player->getCaptain()->getPlayer()->getId());
        if(new_player_node == nullptr){
            delete team_node_by_ability;
            delete player_to_add;
            return StatusType::ALLOCATION_ERROR;
        }
        //update num of games
        player_to_add->setNumOfGamesPlayed(gamesPlayed - root->getPlayer()->getNumOfGamesPlayed() - team_of_player->getNumOfGamesPlayed());
    }
    if(goalKeeper){
        team_of_player->setGoalKeeperStatus(true);
    }
    players_union_find->addNode(new_player_node);
    team_of_player->setNumOfPlayers(team_of_player->getNumOfPlayers() + 1);
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1<=0 || teamId2 <=0 || teamId1==teamId2) return StatusType::INVALID_INPUT;
    AVLRankTreeNode<Team*>* team1_node = teams_tree_by_id->findNode(teamId1);
    if(team1_node== nullptr) return StatusType::FAILURE;
    AVLRankTreeNode<Team*>* team2_node = teams_tree_by_id->findNode(teamId2);
    if(team2_node== nullptr) return StatusType::FAILURE;
    if(!(team1_node->getData()->getGoalKeeperStatus()) || !(team2_node->getData()->getGoalKeeperStatus())) return  StatusType::FAILURE;

    Team* team1 = team1_node->getData();
    Team* team2 = team2_node->getData();

    int team1_score = team1->getPoints() + team1->getTotalAbility();
    int team2_score = team2->getPoints() + team2->getTotalAbility();

    //update num of games for each team
    team1->setNumOfGamesPlayed(team1->getNumOfGamesPlayed() + 1);
    team2->setNumOfGamesPlayed(team2->getNumOfGamesPlayed() + 1);

    //find winner:
    if(team1_score>team2_score){
        //team1 wins by ability
        team1->setPoints(team1->getPoints() + 3);
        return output_t<int>(1);
    }
    else if(team2_score>team1_score){
        //team2 wins by ability
        team2->setPoints(team2->getPoints() + 3);
        return output_t<int>(3);
    }
    else{//abilities are equal
        int team1_strength = team1->getTotalSpirit().strength();
        int team2_strength = team2->getTotalSpirit().strength();

        if(team1_strength>team2_strength){
            //team1 wins by strength
            team1->setPoints(team1->getPoints() + 3);
            return output_t<int>(2);
        }
        if(team2_strength>team1_strength){
            //team2 wins by strength
            team2->setPoints(team2->getPoints() + 3);
            return output_t<int>(4);
        }
        else{
            //tie
            team1->setPoints(team1->getPoints() + 1);
            team2->setPoints(team2->getPoints() + 1);
            return output_t<int>(0);
        }
    }
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
    if(playerId<=0) return StatusType::INVALID_INPUT;
    if(players_union_find->getHashTable()->findElement(playerId) == nullptr) return StatusType::FAILURE;

    PlayerUFNode* node = players_union_find->getHashTable()->findElement(playerId)->getObj();
    int played_games_total = players_union_find->gamesFromNodeToRoot(node);
    return output_t<int>(played_games_total);
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if(playerId<=0 || cards <0 ) return StatusType::INVALID_INPUT;
    if(players_union_find->getHashTable()->findElement(playerId) == nullptr) return StatusType::FAILURE;
    if(players_union_find->findRoot(playerId)->getTeam() == nullptr) return StatusType::FAILURE;

    Player* player_to_update = players_union_find->getHashTable()->findElement(playerId)->getObj()->getPlayer();
    player_to_update->setCards(player_to_update->getCards() + cards);

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
    if(playerId<=0) return StatusType::INVALID_INPUT;
    if(players_union_find->getHashTable()->findElement(playerId) == nullptr) return StatusType::FAILURE;

    Player* player = players_union_find->getHashTable()->findElement(playerId)->getObj()->getPlayer();
    int player_cards = player->getCards();
	return output_t<int>(player_cards);
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId<=0) return StatusType::INVALID_INPUT;
    if(teams_tree_by_id->findNode(teamId) == nullptr) return StatusType::FAILURE;

    Team* team = teams_tree_by_id->findNode(teamId)->getData();
    int team_points = team->getPoints();
	return output_t<int>(team_points);
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{

    int teams_num = teams_tree_by_id->getNumOfNode();
    if(i<0 || teams_num == 0 || i >= teams_num) return StatusType::FAILURE;

    //it is i+1 because the ranks start from 1 and i is from 0
    AVLRankTreeNode<Team*>* team_in_given_index = teams_tree_by_ability->select(i+1);
    if (team_in_given_index == nullptr) return StatusType::FAILURE;

    int team_id = team_in_given_index->getData()->getId();
	return output_t<int>(team_id);
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if(playerId<=0) return StatusType::INVALID_INPUT;
    if(players_union_find->getHashTable()->findElement(playerId) == nullptr) return StatusType::FAILURE;
    if(players_union_find->findRoot(playerId)->getTeam() == nullptr) return StatusType::FAILURE;

    PlayerUFNode* node = players_union_find->getHashTable()->findElement(playerId)->getObj();
    permutation_t partial_spirit = players_union_find->spiritFromNodeToRoot(node);

	return output_t<permutation_t>(partial_spirit);
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
    //team1 is buyer, team2 is bought

    if(teamId1<=0 || teamId2<=0 || teamId1==teamId2) return StatusType::INVALID_INPUT;
    if(teams_tree_by_id->findNode(teamId1) == nullptr) return StatusType::FAILURE;
    if(teams_tree_by_id->findNode(teamId2) == nullptr) return StatusType::FAILURE;

    Team* team1 = teams_tree_by_id->findNode(teamId1)->getData();
    Team* team2 = teams_tree_by_id->findNode(teamId2)->getData();

    if (team2->getNumOfPlayers() != 0) {
        players_union_find->unionTeams(team1, team2);
    }

    //delete the bought team from our structure
    AVLRankTreeNode<Team *> temp1;
    temp1.setSortKey1(team2->getTotalAbility());
    temp1.setSortKey2(teamId2);

    AVLRankTreeNode<Team *> *team_node_by_ability = teams_tree_by_ability->findNode(&temp1);
    AVLRankTreeNode<Team *> *team_node_by_id = teams_tree_by_id->findNode(teamId2);

    teams_tree_by_id->removeNode(team_node_by_id);
    teams_tree_by_ability->removeNode(team_node_by_ability);

    //update ability
    AVLRankTreeNode<Team *> temp2;
    temp2.setSortKey1(team1->getTotalAbility());
    temp2.setSortKey2(teamId1);

    AVLRankTreeNode<Team *> *old_team_node_by_ability = teams_tree_by_ability->findNode(&temp2);
    teams_tree_by_ability->removeNode(old_team_node_by_ability);

    team1->setTotalAbility(team1->getTotalAbility() + team2->getTotalAbility());
    AVLRankTreeNode<Team *> *new_team_node_by_ability =
            new(std::nothrow)AVLRankTreeNode<Team *>(team1, team1->getTotalAbility(), teamId1);
    if (new_team_node_by_ability == nullptr) return StatusType::ALLOCATION_ERROR;

    teams_tree_by_ability->insertNode(new_team_node_by_ability);

    delete team2;
	return StatusType::SUCCESS;
}
