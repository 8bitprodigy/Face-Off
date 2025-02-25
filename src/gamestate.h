/****************************
*                           *
*    G A M E   S T A T E    *
*                           *
****************************/

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <raylib.h>
#include <raymath.h>
#include "thing.h"
#include "actor.h"
#include "player.h"
#include "projectile.h"


typedef enum {
    CO_OP,
    DM,
    TDM,
    CTF
} GameMode;


typedef struct GameState GameState;


/* GameState Constructor */
GameState *GameState_new(GameMode game_mode);
/* GameState Destructor */
void   GameState_free(GameState *game_state);

/* GameState Methods */
/*    Add */
void   GameState_add_Player(GameState *game_state, Player *player);
void   GameState_add_Actor(GameState *game_state, Actor *actor);
void   GameState_add_Thing(GameState *game_state, Thing *thing);
/*    Remove */
void   GameState_remove_Player(GameState *game_state, Player *player);
void   GameState_remove_Actor(GameState *game_state, Actor *actor);
void   GameState_remove_Thing(GameState *game_state, Thing *thing);

/*    Getters */
Map   *GameState_get_Map(GameState *game_state);
float  GameState_get_delta(GameState *game_state);
/*    Setters */
void   GameState_set_paused(GameState *game_state, bool paused);
void   GameState_set_Map(GameState *game_state, Map *map);
/*    Run */
void   GameState_run(GameState *game_state);
/*    Update */
void   GameState_update(GameState *game_state);
/*    Render */
void   GameState_render(GameState *game_state);

#endif /* GAMESTATE_H */
