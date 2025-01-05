/*******************************************************************************
*                                                                              *
*                            F A C E - O F F                                   *
*                                                                              *
********************************************************************************
*                                                                              *
* This is unencumbered software, released into the public domain.              *
* For jurisdictions where the public domain is not applicable, it is released  *
* under the following license:                                                 *
*                                                                              *
* Zero-Clause BSD                                                              *
* ===============                                                              *
*                                                                              *
* Permission to use, copy, modify, and/or distribute this software for         *
* any purpose with or without fee is hereby granted.                           *
*                                                                              *
* THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL                *
* WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES     *
* OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE       *
* FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY           *
* DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   *
* AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT      *
* OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.            *
*                                                                              *
*******************************************************************************/
#include <stdlib.h>
#include <raylib.h>
#include "face-off.h"
#define DEBUG
#include "defs.h"
#include "thing.h"
#include "actor.h"
#include "player.h"
#include "map.h"


static bool done = false;


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


void
GameState_add_Player(GameState *game_state, Player *player)
{
    game_state->num_players++;
    Player_push(&game_state->players, player);
    GameState_add_Actor(game_state, &player->_);
} /* GameState_add_Player */


void
GameState_add_Actor(GameState *game_state, Actor *actor)
{
    game_state->num_actors++;
    Actor_push(&game_state->actors, actor);
    GameState_add_Thing(game_state, &actor->_);
} /* GameState_add_Actor */


void
GameState_add_Thing(GameState *game_state, Thing *thing)
{
    game_state->num_things++;
    Thing_push(&game_state->things, thing);
} /* GameState_add_Thing */


void
GameState_remove_Player(GameState *game_state, Player *player)
{
    game_state->num_players--;
    Player_pop(player);
    GameState_remove_Actor(game_state, &player->_);
} /* GameState_remove_Player */


void
GameState_remove_Actor(GameState *game_state, Actor *actor)
{
    game_state->num_actors--;
    Actor_pop(actor);
    GameState_remove_Thing(game_state, &actor->_);
} /* GameState_remove_Actor */


void
GameState_remove_Thing(GameState *game_state, Thing *thing)
{
    game_state->num_things--;
    Thing_pop(thing);
} /* GameState_remove_Thing */


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
    
    DBG_OUT("Actor: %u", actor);
    while (player != &game_state->players) {
        Player_update(player, delta, game_state);
        player = player->next;
    }
    while (actor != &game_state->actors) {
        actor->update(actor, delta, game_state);
        actor = actor->next;
    }
}


int 
main(/*int argc, char** argv*/) 
{
    Camera camera = {0};
    Player *player;
    Map    *map;
    GameState *game_state;
    
    InitWindow (SCREEN_WIDTH, SCREEN_HEIGHT, "Face-Off!");

    camera.position   = (Vector3){ 1.0f, 0.75f, 0.0f};
    camera.target     = (Vector3){ 0.0f, 0.75f, 0.0f};
    camera.up         = (Vector3){ 0.0f, 1.0f,  0.0f};
    camera.fovy       = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    player = Player_new(
        Vector2Zero(),
        0.0f, 0.5f, 0
    );

    map = Map_new("test", 16, 4);

    game_state = GameState_new(CO_OP);
    GameState_add_Player(game_state, player);
    game_state->map = map;

    SetTargetFPS(60);
    
    while (!done) {
        done = GET_KEY_OR_BUTTON_DOWN(0, GAMEPAD_BUTTON_MIDDLE_RIGHT, KEY_ESCAPE);
        //UpdateCamera( &camera, CAMERA_ORBITAL);
        GameState_update(game_state);
        camera = player->camera;
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawFPS(10,10);
            BeginMode3D(camera);

                DrawGrid(16, 4.0f);
                /*
                DrawCube((Vector3){  16.5f, -0.55f,   0.0f},  1.0f, 1.0f, 32.0f, RED);
                DrawCube((Vector3){   0.0f, -0.55f,  16.5f}, 32.0f, 1.0f,  1.0f, GREEN);
                DrawCube((Vector3){ -16.5f, -0.55f,   0.0f},  1.0f, 1.0f, 32.0f, BLUE);
                DrawCube((Vector3){   0.0f, -0.55f, -16.5f}, 32.0f, 1.0f,  1.0f, YELLOW);
                */
                Map_render(map, player);
                DrawSphere(VECTOR2_TO_3( player->_._.position, CAMERA_HEIGHT ),0.5f,YELLOW);
                DBG_LINE(player->_._.position, VECTOR3_TO_2(camera.target), CAMERA_HEIGHT, BLACK);
                //DrawSphere(camera.target,0.25f,BLACK);
                
            EndMode3D();
            
        EndDrawing();
        
    }

    CloseWindow();

    Player_free(player);
    Map_free(map);
    
    return 0;
} /* main */
