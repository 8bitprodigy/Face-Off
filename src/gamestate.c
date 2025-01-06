#include <stdlib.h>
#include "gamestate_private.h"
#define DEBUG
#include "defs.h"



/******************************
*    C O N S T R U C T O R    *
******************************/
GameState
*GameState_new(GameMode game_mode)
{
    Player *players;
    Actor  *actors;
    Thing  *things;
    GameState *game_state = malloc(sizeof(GameState));
    
    if(!game_state) {
        ERR_OUT("Failed to allocate memory for GameState.");
        return NULL;
    }

    players = &game_state->players;
    actors  = &game_state->actors;
    things  = &game_state->things;
    
    game_state->game_mode = game_mode;
    game_state->paused    = false;

    players->prev           = players;
    players->next           = players;
    game_state->num_players = 0;

    actors->prev           = actors;
    actors->next           = actors;
    game_state->num_actors = 0;

    things->prev           = things;
    things->next           = things;
    game_state->num_things = 0;
    
    return game_state;
} /* GameState_new */

/****************************
*    D E S T R U C T O R    *
****************************/
void
GameState_free(GameState *game_state)
{
    while (game_state->players.prev != game_state->players.next) {
        Player_free(game_state->players.prev);
    }
    while (game_state->actors.prev != game_state->actors.next) {
        Actor_free(game_state->actors.prev);
    }
    while (game_state->things.prev != game_state->things.next) {
        Thing_free(game_state->things.prev);
    }
    Map_free(game_state->map);
    free(game_state);
} /* GameState_free */


/****************************
*    O P E R A T I O N S    *
****************************/
/*    A D D    */
void
GameState_add_Player(GameState *game_state, Player *player)
{
    game_state->num_players++;
    Player_push(&game_state->players, player);
    GameState_add_Actor(game_state, &player->base);
} /* GameState_add_Player */

void
GameState_add_Actor(GameState *game_state, Actor *actor)
{
    game_state->num_actors++;
    Actor_push(&game_state->actors, actor);
    GameState_add_Thing(game_state, &actor->base);
} /* GameState_add_Actor */

void
GameState_add_Thing(GameState *game_state, Thing *thing)
{
    game_state->num_things++;
    Thing_push(&game_state->things, thing);
} /* GameState_add_Thing */


/*    R E M O V E    */
void
GameState_remove_Player(GameState *game_state, Player *player)
{
    game_state->num_players--;
    Player_pop(player);
    GameState_remove_Actor(game_state, &player->base);
} /* GameState_remove_Player */

void
GameState_remove_Actor(GameState *game_state, Actor *actor)
{
    game_state->num_actors--;
    Actor_pop(actor);
    GameState_remove_Thing(game_state, &actor->base);
} /* GameState_remove_Actor */

void
GameState_remove_Thing(GameState *game_state, Thing *thing)
{
    game_state->num_things--;
    Thing_pop(thing);
} /* GameState_remove_Thing */


/**********************
*    G E T T E R S    *
**********************/
Map
*GameState_get_Map(GameState *game_state)
{
    return game_state->map;
}


/**********************
*    S E T T E R S    *
**********************/
void
GameState_set_Map(GameState *game_state, Map *map)
{
    game_state->map = map;
}

/********************
*    U P D A T E    *
********************/
void
GameState_update(GameState *game_state)
{
    //int i;
    
    float delta;
    
    Player *player;
    Actor  *actor;
    //Thing  *thing;

    delta = GetFrameTime();
    
    player = game_state->players.next;
    actor  = game_state->actors.next;
    //thing  = game_state->things.next;
    
    //DBG_OUT("Actor: %u", actor);
    while (player != &game_state->players) {
        Player_update(player, delta, game_state);
        player = player->next;
    }
    while (actor != &game_state->actors) {
        actor->update(actor, delta, game_state);
        actor = actor->next;
    }
}


