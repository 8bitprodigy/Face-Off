#include <stdlib.h>
#include <math.h>

//#define DEBUG
#include "defs.h"
#include "gamestate.h"
#include "player_private.h"


/****************************
    C O N S T R U C T O R   
****************************/
void
Player_init(Player *player, Vector2 position, float rotation, float radius, int controller)
{
    Thing  *thing;
    Actor  *actor;
    
    actor = ACTOR(player);
    thing = THING(player);
    
    Actor_init(actor, BODY_NONE(), position, rotation, radius);
    actor->update = &Player_update;
    
    player->prev = player;
    player->next = player;
    
    player->controller = controller;

    player->camera.position   = (Vector3){.x=0.0f,.y=CAMERA_HEIGHT,.z=0.0f};
    player->camera.target     = (Vector3){ 
        .x = position.x + thing->cos_rot,
        .y = CAMERA_HEIGHT,
        .z = position.y + thing->sin_rot
    };
    player->camera.up         = VECTOR3_UP;
    player->fov               = PI/2.0f;
    player->half_fov          = player->fov/2.0f;
    player->camera.fovy       = 2 * atan(
        tan(player->half_fov) /
        ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)
    ) * 180 / PI;
    player->camera.projection = CAMERA_PERSPECTIVE;
} /* Player_init */

Player *
Player_new( Vector2 position, float rotation, float radius, int controller)
{
    Player *player = malloc(sizeof(Player));
    if (!player) {
        ERR_OUT("Failed to allocate memory for Player.");
        return NULL;
    }

    Player_init(player, position, rotation, radius, controller);

    return player;
} /* Player_new */


/**************************
    D E S T R U C T O R    
**************************/
void
Player_free(Player *player)
{
    Thing_remove(THING(player));
    Actor_remove(ACTOR(player));
    Player_remove(player);
    free(player);
} /* Player_free */


/********************
    G E T T E R S    
********************/
Camera *
Player_get_Camera(Player *player)
{
    return &player->camera;
} /* Player_get_Camera */

Vector2
Player_get_position(Player *player)
{
    return player->base.base.position;
} /* Player_get_position */

float
Player_get_half_fov(Player *player)
{
    return player->half_fov;
} /* Player_get_half_fov */

Player *
Player_get_prev(Player *player)
{
    return player->prev;
}


/*******************
   M E T H O D S
*******************/
/*    L I S T   O P E R A T I O N S    */
/*        A D D    */
void
Player_insert(Player *player1, Player *player2)
{
    Player *player3 = player1->prev;

    player3->next = player2;
    player1->prev = player2;
    
    player2->next = player1;
    player2->prev = player3;
} /* Player_insert */

/*        R E M O V E    */
void
Player_remove(Player *player)
{
    Player *player1 = player->prev;
    Player *player2 = player->next;

    player1->next = player2;
    player2->prev = player1;
} /* Player_remove */


/******************
    U P D A T E
******************/
void 
Player_update(Actor *actor, GameState *game_state)
{
    Player *self = PLAYER(actor);
    Thing  *thing  = THING(actor);
    
    float rotate   = 0;

    Vector2 move;
    Vector2 target;

    rotate = GET_KEY_OR_BUTTON_AXIS(
        self->controller,
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,
        KEY_RIGHT,
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT,
        KEY_LEFT ) * actor->turn_speed;
    actor->angular_velocity = rotate;
    Actor_rotate(actor, game_state);
    
    move = Vector2Scale(
        GET_KEY_OR_BUTTON_VECTOR(
            self->controller,
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

    move = Vector2Rotate(move,thing->rotation);

    Actor_move(actor, move, game_state);

    self->camera.position = VECTOR2_TO_3( thing->position, CAMERA_HEIGHT ); // 30.0f
    target                = Vector2Add(thing->position, (Vector2){ thing->cos_rot, thing->sin_rot });
    self->camera.target   = VECTOR2_TO_3( target, CAMERA_HEIGHT );

    if (GET_KEY_OR_BUTTON_PRESSED(self->controller,GAMEPAD_BUTTON_RIGHT_FACE_UP,KEY_SPACE)) {
        DBG_OUT("Shooting...");
        Actor_shoot(actor, game_state);
    }
} /* Player_update */
