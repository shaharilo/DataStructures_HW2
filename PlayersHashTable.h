
#ifndef DATA_STRUCTURES_WET2_PLAYERSHASHTABLE_H
#define DATA_STRUCTURES_WET2_PLAYERSHASHTABLE_H

#include "HashTable.h"
#include "PlayerUFNode.h"

class PlayersHashTable : public HashTable<PlayerUFNode*>{
public:
    PlayersHashTable(): HashTable<PlayerUFNode*>(){};
    virtual ~PlayersHashTable() = default;
};


#endif //DATA_STRUCTURES_WET2_PLAYERSHASHTABLE_H
