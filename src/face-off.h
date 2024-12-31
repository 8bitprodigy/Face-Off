#ifndef FACE_OFF_H
#define FACE_OFF_H

#include "player.h"
#include "item.h"
#include "map.h"
#include "defs.h"


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
    uint     num_players;
    Player   players[GAME_MAX_PLAYERS];
    uint     num_actors;
    Actor    actors[GAME_MAX_ACTORS];
    uint     num_things;
    Thing    things[GAME_MAX_THINGS];
    Map      *map;
    float    delta;
    bool     paused;
} GameState;

/* GameState Constructor */
GameState *GameState_new(void);
/* GameState Destructor */
void GameState_free(GameState *game_state);

/* GameState Update */
void GameState_update(void);
/* GameState Render */
void GameState_render(void);

#endif /* FACE_OFF_H */
