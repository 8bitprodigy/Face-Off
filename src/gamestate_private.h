#ifndef GAMESTATE_PRIVATE_H
#define GAMESTATE_PRIVATE_H

#include "gamestate.h"
#include "thing_private.h"
#include "actor_private.h"
#include "player_private.h"
#include "map.h"


typedef struct 
GameState
{
    GameMode game_mode;
    
    Player   players;
    uint     num_players;
    
    Actor    actors;
    uint     num_actors;
    
    Thing    things;
    uint     num_things;
    
    Map      *map;
    bool     paused;
} GameState;
    
#endif /* GAMESTATE_PRIVATE_H */
