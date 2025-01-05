#include <stdlib.h>
#include <math.h>
#define DEBUG
#include "defs.h"
#include "player.h"

Player 
*Player_new(Vector2 position, float rotation, float radius, int controller)
{
    Thing  *thing;
    Actor  *actor;
    Player *player = malloc(sizeof(Player));
    if (!player) {
        ERR_OUT("Failed to allocate memory for Player.");
        return NULL;
    }

    actor = &player->_;
    thing = &actor->_;
    
    *thing = (Thing){
        .position = position,
        .rotation = rotation,
        .sin_rot  = sin(rotation),
        .cos_rot  = cos(rotation),
        .radius   = radius,
    };

    *actor = (Actor){
        .type             = PLAYER,
        .prev_pos         = Vector2Zero(),
        .velocity         = Vector2Zero(),
        .speed            = 5.0f,
        .turn_speed       = 1.0f,
        .prev_rot         = 0.0f,
        .angular_velocity = 0.0f,
        .health           = 4,
        .update           = &Actor_move,
    };

    player->controller = controller;

    player->camera.position   = (Vector3){.x=0.0f,.y=CAMERA_HEIGHT,.z=0.0f};
    
    player->camera.target     = (Vector3){ 
        .x = position.x + thing->cos_rot,
        .y = CAMERA_HEIGHT,
        .z = position.y + thing->sin_rot
    };
    DBG_OUT("Camera target: { X: %.4f,\tY: %.4f }", player->camera.target.x, player->camera.target.y);
    player->camera.up         = VECTOR3_UP;
    player->fov               = PI/2.0f;
    player->half_fov          = player->fov/2.0f;
    player->camera.fovy       = 2 * atan(
        tan(player->half_fov) /
        ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)
    ) * 180 / PI;
    player->camera.projection = CAMERA_PERSPECTIVE;

    return player;
} /* Player_new */


void
Player_free(Player *player)
{
    Thing_pop(&player->_._);
    Actor_pop(&player->_);
    Player_pop(player);
    free(player);
} /* Player_free */


void
Player_push(Player *player1, Player *player2)
{
    Player *player3 = player1->prev;

    player3->next = player2;
    player1->prev = player2;
    
    player2->next = player1;
    player2->prev = player3;

    //Actor_push(&player1->_, &player2->_);
} /* Player_push */


void
Player_pop(Player *player)
{
    Player *player1 = player->prev;
    Player *player2 = player->next;

    player1->next = player2;
    player2->prev = player1;

    //Actor_pop(&player->_);
} /* Player_pop */


void 
Player_update(Player *player, float delta, GameState *game_state)
{
    Actor *actor = &player->_;
    Thing *thing = &actor->_;
    
    float rotate = 0;

    Vector2 move;
    Vector2 target;

    rotate = GET_KEY_OR_BUTTON_AXIS(
        player->controller,
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,
        KEY_RIGHT,
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT,
        KEY_LEFT ) * actor->turn_speed;
    actor->angular_velocity = rotate;
    Actor_rotate(actor, delta);
    
    move = Vector2Scale(
        GET_KEY_OR_BUTTON_VECTOR(
            player->controller,
            GAMEPAD_BUTTON_LEFT_FACE_UP,
            KEY_W,
            GAMEPAD_BUTTON_LEFT_FACE_DOWN,
            KEY_S,
            GAMEPAD_BUTTON_LEFT_FACE_RIGHT,
            KEY_D,
            GAMEPAD_BUTTON_LEFT_FACE_LEFT,
            KEY_A 
        ), 
        actor->speed );

    actor->velocity = Vector2Rotate(move,thing->rotation);

    //Actor_move(actor, delta, game_state);

    player->camera.position   = VECTOR2_TO_3( thing->position, CAMERA_HEIGHT );
    target = Vector2Add(thing->position, (Vector2){ thing->cos_rot, thing->sin_rot });
    player->camera.target     = VECTOR2_TO_3( target, CAMERA_HEIGHT );
} /* Player_update */
