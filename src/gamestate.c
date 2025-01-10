/****************************
*                           *
*    G A M E   S T A T E    *
*                           *
****************************/

#include <stdlib.h>
#include "gamestate.h"
#include "thing_private.h"
#include "actor_private.h"
#include "player_private.h"
#include "map.h"
#define DEBUG
#include "defs.h"


typedef struct 
GameState
{
    GameMode game_mode;
    
    Actor    *actors;
    uint     num_actors;
    
    Thing    *things;
    uint     num_things;
    
    Map      *map;
    bool     paused;

    float    delta;
} GameState;

/****************************
    C O N S T R U C T O R    
****************************/
GameState
*GameState_new(GameMode game_mode)
{
    Actor  *actors;
    Thing  *things;
    GameState *game_state = malloc(sizeof(GameState));
    
    if(!game_state) {
        ERR_OUT("Failed to allocate memory for GameState.");
        return NULL;
    }

    game_state->num_actors = 0;
    game_state->num_things = 0;
    
    game_state->game_mode = game_mode;
    game_state->paused    = false;
    
    return game_state;
} /* GameState_new */

/**************************
    D E S T R U C T O R    
**************************/
void
GameState_free(GameState *game_state)
{
    while (game_state->actors) {
        Actor_free(game_state->actors->prev);
    }
    while (game_state->things)  {
        Thing_free(game_state->things->prev);
    }
    Map_free(game_state->map);
    free(game_state);
} /* GameState_free */


/************************************
    L I S T   O P E R A T I O N S    
************************************/
/*    A D D    */
void 
GameState_add_Player(GameState *game_state, Player *player)
{
    GameState_add_Actor(game_state, ACTOR(player));
}

void
GameState_add_Actor(GameState *game_state, Actor *actor)
{
    game_state->num_actors++;
    if (!game_state->actors) game_state->actors = actor;
    Actor_insert(game_state->actors, actor);
    //GameState_add_Thing(game_state, &actor->base);
} /* GameState_add_Actor */

void
GameState_add_Thing(GameState *game_state, Thing *thing)
{
    game_state->num_things++;
    if (!game_state->things) game_state->things = thing;
    Thing_insert(game_state->things, thing);
} /* GameState_add_Thing */


/*    R E M O V E    */
void 
GameState_remove_Player(GameState *game_state, Player *player)
{
    GameState_remove_Actor(game_state, ACTOR(player));
}

void
GameState_remove_Actor(GameState *game_state, Actor *actor)
{
    game_state->num_actors--;
    Actor_remove(actor);
    GameState_remove_Thing(game_state, &actor->base);
} /* GameState_remove_Actor */

void
GameState_remove_Thing(GameState *game_state, Thing *thing)
{
    game_state->num_things--;
    Thing_remove(thing);
} /* GameState_remove_Thing */


/********************
    G E T T E R S    
********************/
float
GameState_get_delta(GameState *game_state)
{
    return game_state->delta;
}

Map
*GameState_get_Map(GameState *game_state)
{
    return game_state->map;
}


/********************
    S E T T E R S    
********************/
void
GameState_set_Map(GameState *game_state, Map *map)
{
    game_state->map = map;
}


/******************
    U P D A T E    
******************/
void
GameState_update(GameState *game_state)
{
    int i;
    float delta;
    
    Actor *actor;
    uint  num_actors = game_state->num_actors;

    delta = GetFrameTime();
    game_state->delta = delta;
    
    actor  = game_state->actors->next;
    
    for (i = num_actors; 0 < i; i--) {
        actor->update(actor, game_state);
        actor = actor->next;
    }
}


