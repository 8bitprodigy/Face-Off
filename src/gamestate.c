/****************************
*                           *
*    G A M E   S T A T E    *
*                           *
****************************/

#include <assert.h>
#include <stdlib.h>

//#define DEBUG
#include "defs.h"
#include "gamestate.h"
#include "map.h"


typedef struct 
GameState
{
    GameMode  game_mode;

    Player   *players;
    uint      num_players;
    
    Actor    *actors;
    uint      num_actors;
    
    Thing    *things;
    uint      num_things;
    
    Map      *map;
    bool      paused;
    bool      request_exit;

    float     delta;
} GameState;

/****************************
    C O N S T R U C T O R    
****************************/
GameState *
GameState_new(GameMode game_mode)
{
    GameState *game_state = malloc(sizeof(GameState));
    
    if(!game_state) {
        ERR_OUT("Failed to allocate memory for GameState.");
        return NULL;
    }
    
    game_state->game_mode    = game_mode;
    game_state->paused       = false;
    game_state->request_exit = false;
    game_state->num_actors   = 0;
    game_state->num_things   = 0;
    
    // Initialize list heads to NULL
    game_state->players = NULL; 
    game_state->actors  = NULL;
    game_state->things  = NULL;
    
    return game_state;
} /* GameState_new */

/**************************
    D E S T R U C T O R    
**************************/
void
GameState_free(GameState *game_state)
{
    Player *players = game_state->players;
    Actor  *actors  = game_state->actors;
    Thing  *things  = game_state->things;

    while (players) {
        Player_free(Player_get_prev(players));
    }
    while (actors) {
        Actor_free(Actor_get_prev(actors));
    }
    while (things) {
        Thing_free(Thing_get_prev(things));
    }
    
    Map_free(game_state->map);
    free(game_state);
} /* GameState_free */


/************************************
    L I S T   O P E R A T I O N S    
************************************/
/*    A D D    */
void 
GameState_add_Player(GameState *self, Player *player)
{
    self->num_players++;
    if (!self->players) {
        self->players = player;
    } else {
        Player_insert(self->players, player);
    }
    GameState_add_Actor(self, ACTOR(player));
} /* GameState_add_Player */

void
GameState_add_Actor(GameState *self, Actor *actor)
{
    self->num_actors++;
    if (!self->actors) {
        self->actors = actor;
    } else {
        Actor_insert(self->actors, actor);
    }
    GameState_add_Thing(self, THING(actor));
} /* GameState_add_Actor */

void 
GameState_add_Thing(GameState *self, Thing *thing)
{
    self->num_things++;
    if (!self->things) {
        self->things = thing;
    } else {
        Thing_insert(self->things, thing);
    }
} /* GameState_add_Thing */


/*    R E M O V E    */
void 
GameState_remove_Player(GameState *self, Player *player)
{
    self->num_players--;
    Player_remove(player);
    GameState_remove_Actor(self, ACTOR(player));
} /* GameState_remove_Player */

void
GameState_remove_Actor(GameState *self, Actor *actor)
{
    self->num_actors--;
    Actor_remove(actor);
    GameState_remove_Thing(self, THING(actor));
} /* GameState_remove_Actor */

void
GameState_remove_Thing(GameState *self, Thing *thing)
{
    self->num_things--;
    Thing_remove(thing);
} /* GameState_remove_Thing */


/********************
    G E T T E R S    
********************/
float
GameState_get_delta(GameState *self)
{
    return self->delta;
} /* GameState_get_delta */

Map *
GameState_get_Map(GameState *self)
{
    return self->map;
} /* GameState_get_Map */


/********************
    S E T T E R S    
********************/
void
GameState_set_paused(GameState *self, bool paused)
{
    self->paused = paused;
} /* GameState_set_paused */

void
GameState_set_Map(GameState *self, Map *map)
{
    self->map = map;
} /* GameState_set_Map */


/************
    R U N   
************/
void
GameState_run(GameState *self)
{
    while(!self->request_exit) {
        if (self->paused) continue;
        GameState_update(self);
        GameState_render(self);
        self->request_exit = (
            WindowShouldClose() 
            || GET_KEY_OR_BUTTON_DOWN(0, GAMEPAD_BUTTON_MIDDLE_RIGHT, KEY_ESCAPE)
        );
    }
} /* GameState_run */


/******************
    U P D A T E    
******************/
void
GameState_update(GameState *self)
{
    Actor *actors = self->actors;
    Actor *actor;
    
    self->delta =  GetFrameTime();;
    
    actor  = actors;
    do {
        assert(actor);
        Actor_update(actor, self);
        actor = Actor_get_next(actor);
    } while (Actor_get_next(actor) != actors);
} /* GameState_update */


/******************
    R E N D E R    
******************/
void
GameState_render(GameState *self)
{ /* The following is temporary and will be replaced later with something that will support multiplayer... */
    BeginDrawing();
        
        ClearBackground(RAYWHITE);
        DrawFPS(10,10);
        
        BeginMode3D(*Player_get_Camera(self->players));
    
            DrawGrid(16, 4.0f);
            
            Map_render(self->map, self->players);
            DrawSphere(VECTOR2_TO_3( Player_get_position(self->players), CAMERA_HEIGHT ),0.5f,YELLOW);
            
        EndMode3D();
    EndDrawing();
} /* GameState_render */
