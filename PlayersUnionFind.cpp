//
// Created by Toam Elharar on 21/12/2022.
//

#include "PlayersUnionFind.h"
#define INIT_PARENT_VALUE -1


PlayersUnionFind::PlayersUnionFind():num_of_players(0) {
    hash_table = new PlayersHashTable();
}

PlayersUnionFind::~PlayersUnionFind() {

    List<PlayerUFNode*>** tabel = hash_table->getTable();
    for(int i = 0; i < hash_table->getTableSize(); i++) {
        Node<PlayerUFNode*>* head = tabel[i]->getHead();
        while (head != nullptr) {
            PlayerUFNode* player_Uf_Node = head->getObj();
            if(player_Uf_Node != nullptr){
                head->setObj(NULL);
                Player* current_player = player_Uf_Node->getPlayer();
                if (current_player != nullptr) {
                    delete current_player;
                }
                delete player_Uf_Node;
            }
            head = head->getNext();
        }
    }

    delete hash_table;
}

PlayersHashTable* PlayersUnionFind::getHashTable(){
    return this->hash_table;
}

int PlayersUnionFind::getNumOfPlayers(){
    return this->num_of_players;
}

void PlayersUnionFind::setHashTable(PlayersHashTable* hash_table){
    this->hash_table = hash_table;
}

void PlayersUnionFind::setNumOfPlayers(int num){
    this->num_of_players = num;
}

void PlayersUnionFind::addNode(PlayerUFNode* new_player_node){
    hash_table->insertNewElement(new_player_node, new_player_node->getPlayer()->getId());
}

void PlayersUnionFind::unionTeams(Team* buyerTeam, Team* boughtTeam) {

    if (!buyerTeam->getCaptain() && !boughtTeam->getCaptain()){
        return;
    }

    if(buyerTeam->getNumOfPlayers() >= boughtTeam->getNumOfPlayers()){
        if (boughtTeam->getNumOfPlayers() == 0){
            return;
        }

        Player* old_captain = boughtTeam->getCaptain()->getPlayer();
        Player* new_captain = buyerTeam->getCaptain()->getPlayer();

        //update num of games
        boughtTeam->getCaptain()->setParentId(buyerTeam->getCaptain()->getPlayer()->getId());
        old_captain->setNumOfGamesPlayed(old_captain->getNumOfGamesPlayed() + boughtTeam->getNumOfGamesPlayed() -
                                                 new_captain->getNumOfGamesPlayed() - buyerTeam->getNumOfGamesPlayed());

        //update spirit
        permutation_t temp_calculation = (new_captain->getPartialSpirit().inv() * buyerTeam->getTotalSpirit());
        old_captain->setPartialSpirit(temp_calculation * old_captain->getPartialSpirit());
        buyerTeam->setTotalSpirit(buyerTeam->getTotalSpirit() * boughtTeam->getTotalSpirit());

        boughtTeam->getCaptain()->setTeam(nullptr);
    }
    else {
        //define new captain for the buying team
        PlayerUFNode *new_captain_node = boughtTeam->getCaptain();
        Player *new_captain = new_captain_node->getPlayer();

        if (buyerTeam->getNumOfPlayers() != 0) {
            Player *old_captain = buyerTeam->getCaptain()->getPlayer();
            //update num of games
            buyerTeam->getCaptain()->setParentId(boughtTeam->getCaptain()->getPlayer()->getId());

            old_captain->setNumOfGamesPlayed(old_captain->getNumOfGamesPlayed()
            + buyerTeam->getNumOfGamesPlayed() - new_captain->getNumOfGamesPlayed() - boughtTeam->getNumOfGamesPlayed());
            new_captain->setNumOfGamesPlayed(new_captain->getNumOfGamesPlayed()
            + boughtTeam->getNumOfGamesPlayed() - buyerTeam->getNumOfGamesPlayed());

            buyerTeam->getCaptain()->setTeam(nullptr);

            //update spirits
            new_captain->setPartialSpirit(  buyerTeam->getTotalSpirit() * new_captain->getPartialSpirit());
            old_captain->setPartialSpirit((new_captain->getPartialSpirit()).inv() * old_captain->getPartialSpirit());
        }
        else{
            new_captain->setNumOfGamesPlayed(new_captain->getNumOfGamesPlayed() + boughtTeam->getNumOfGamesPlayed());
        }
        new_captain_node->setTeam(buyerTeam);
        buyerTeam->setCaptain(new_captain_node);
        buyerTeam->setTotalSpirit(buyerTeam->getTotalSpirit() * boughtTeam->getTotalSpirit());
    }

        buyerTeam->setPoints(buyerTeam->getPoints() + boughtTeam->getPoints());
        buyerTeam->setNumOfPlayers(buyerTeam->getNumOfPlayers() + boughtTeam->getNumOfPlayers());
        boughtTeam->setCaptain(nullptr);
        buyerTeam->setGoalKeeperStatus(buyerTeam->getGoalKeeperStatus() || boughtTeam->getGoalKeeperStatus());
}

PlayerUFNode* PlayersUnionFind::findRoot(int key) {
    PlayerUFNode* player_node_in_UF = hash_table->findElement(key)->getObj();

    //finding root
    PlayerUFNode* root = player_node_in_UF;
    while(root->getParentId() != INIT_PARENT_VALUE){ // -1 is the value if there is no parent
        int parent_id = root->getParentId();
        PlayerUFNode* parent = hash_table->findElement(parent_id)->getObj();
        root = parent;
    }

    //shrinking tracks
    PlayerUFNode* current_node = player_node_in_UF;
    int next_node_id = current_node->getParentId();

    PlayerUFNode* next_node;
    if (current_node->getParentId() != INIT_PARENT_VALUE){
        next_node = hash_table->findElement(next_node_id)->getObj();
    }
    else {
        next_node = nullptr;
    }

    while(current_node->getParentId() != INIT_PARENT_VALUE && next_node != root){

        Player* curr_player = current_node->getPlayer();
        current_node->setParentId(root->getPlayer()->getId());

        //update played games
        Team* team = root->getTeam();
        int team_games = (team == nullptr)? 0: team->getNumOfGamesPlayed();
        curr_player->setNumOfGamesPlayed(curr_player->getNumOfGamesPlayed() + this->gamesFromNodeToRoot(next_node)
        - root->getPlayer()->getNumOfGamesPlayed() - team_games);

        //update spirit
        permutation_t curr_partial_spirit = curr_player->getPartialSpirit();
        curr_player->setPartialSpirit((root->getPlayer()->getPartialSpirit()).inv() *
        this->spiritFromNodeToRoot(next_node) * curr_partial_spirit);

        current_node = hash_table->findElement(next_node_id)->getObj();
        next_node_id = current_node->getParentId();
        next_node = this->getHashTable()->findElement(next_node_id) ->getObj();
    }
    return root;
}


int PlayersUnionFind::gamesFromNodeToRoot(PlayerUFNode* node) {
    int played_games_total = 0;
    PlayerUFNode* curr_player_node = node;
    while(curr_player_node->getParentId() != -1){
        played_games_total += curr_player_node->getPlayer()->getNumOfGamesPlayed();
        int parent_id = curr_player_node->getParentId();
        Node<PlayerUFNode *> *temp_node = this->getHashTable()->findElement(parent_id);
        if (temp_node == nullptr) break;
        else curr_player_node = temp_node->getObj();
    }
    played_games_total += curr_player_node->getPlayer()->getNumOfGamesPlayed();

    Team* team_pointer = curr_player_node->getTeam();
    if (team_pointer != nullptr){
        int team_games = team_pointer->getNumOfGamesPlayed();
        played_games_total += team_games;
    }
    return played_games_total;
}

permutation_t PlayersUnionFind::spiritFromNodeToRoot(PlayerUFNode* node)
{
    PlayerUFNode* curr_player_node = node;

    permutation_t partial_spirit = curr_player_node->getPlayer()->getPartialSpirit();
    if (curr_player_node->getParentId() == -1) {
        return partial_spirit;
    }

    while(curr_player_node->getParentId() != -1){
        int parent_id = curr_player_node->getParentId();
        Node<PlayerUFNode *> *temp_node = this->getHashTable()->findElement(parent_id);
        if (temp_node == nullptr){
            break; //doesn't suppose to go here;
        }
        else{
            curr_player_node = temp_node->getObj();
        }
        partial_spirit = curr_player_node->getPlayer()->getPartialSpirit() * partial_spirit;
    }
    return partial_spirit;
}






//
//void PlayersUnionFind::printUF(){
//    cout << "Printing UnionFind: " << endl;
//    for (int i = 0; i < hash_table->getTableSize(); i++)
//    {
//        cout << "|" << i << "|";
//        List<PlayerUFNode*>** table = hash_table->getTable();
//        Node<PlayerUFNode*>* curr_node = table[i]->getHead();
//        while (curr_node != nullptr) {
//            cout << "-->" << "list node key: " << curr_node->getKey() <<
//            " ,Player Team: ";
//            if (curr_node->getObj()->getTeam()){
//                Team* team = curr_node->getObj()->getTeam();
//                cout<< team->getId() << " ,Toatal games: " << team->getNumOfGamesPlayed();
//            }
//            else cout <<"NO TEAM";
//            cout << " ,Player Parent ID: " <<curr_node->getObj()->getParentId() << " ,games: " << curr_node->getObj()->getPlayer()->getNumOfGamesPlayed() << endl;
//
//            curr_node = curr_node->getNext();
//        }
//
//        cout << endl;
//    }
//}



