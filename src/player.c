#include <math.h>
#include "player.h"
#include "defs.h"

Player 
*Player_new(Vector2 position, float rotation, float radius, int controller)
{
    Player *player = malloc(sizeof(Player));

    Thing thing = (Thing){
        .position = position,
        .rotation = rotation,
        .sin_rot  = sin(rotation),
        .cos_rot  = cos(rotation),
        .radius   = radius,
    };
    thing.position.x = 0.0f;
    thing.position.y = 0.0f;

    player->_ = (Actor){
        ._          = thing,
        .prev_pos   = position,
        .velocity   = Vector2Zero(),
        .speed      = 5.0f,
        .turn_speed = 1.0f,
        .prev_rot   = rotation,
        .angular_velocity = 0.0f,
    };

    player->controller = controller;

    player->camera.position   = (Vector3){ 
        position.x, 
        CAMERA_HEIGHT, 
        position.y
    };
    player->camera.target     = (Vector3){ 
        position.x + thing.cos_rot,
        CAMERA_HEIGHT,
        position.y + thing.sin_rot
    };
    player->camera.up         = (Vector3){ 0.0f, 1.0f,  0.0f};
    player->camera.fovy       = 45.0f;
    player->camera.projection = CAMERA_PERSPECTIVE;

    return player;
}

void 
Player_update(Player *player, float delta)
{
    Actor *actor = &player->_;
    Thing *thing = &actor->_;
    
    float rotate = 0;

    Vector2 move;
    Vector2 offset;
    Vector2 target;

    rotate = Get_Key_Or_Button_Axis(
        player->controller,
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,
        KEY_RIGHT,
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT,
        KEY_LEFT ) * actor->turn_speed;

    actor->angular_velocity = rotate;
    Actor_rotate(actor, delta);
    
    move = Vector2Scale(
        Get_Key_Or_Button_Vector(
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

    Actor_move(actor, delta);

    player->camera.position   = Vector2_To_3( thing->position, CAMERA_HEIGHT );
    target = Vector2Add(thing->position, (Vector2){ thing->cos_rot, thing->sin_rot });
    player->camera.target     = Vector2_To_3( target, CAMERA_HEIGHT );
}

