#ifndef FACE_OFF_H
#define FACE_OFF_H

#include "player.h"
#include "item.h"
#include "map.h"


typedef enum {
    CO_OP,
    DM,
    TDM,
    CTF
} GameMode;

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

/* GameState Constructor */
GameState *GameState_new(GameMode game_mode);
/* GameState Destructor */
void GameState_free(GameState *game_state);

/* GameState Operations */
/*    Add */
void GameState_add_Player(GameState *game_state, Player *player);
void GameState_add_Actor(GameState *game_state, Actor *actor);
void GameState_add_Thing(GameState *game_state, Thing *thing);
/*    Remove */
void GameState_remove_Player(GameState *game_state, Player *player);
void GameState_remove_Actor(GameState *game_state, Actor *actor);
void GameState_remove_Thing(GameState *game_state, Thing *thing);


/* GameState Update */
void GameState_update(GameState *game_state);
/* GameState Render */
void GameState_render(GameState *game_state);

#endif /* FACE_OFF_H */
