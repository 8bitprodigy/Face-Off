#include <math.h>
#include "player.h"

Player *Player_new(Vector2 position, float rotation, float radius, int controller){
    Player *player = malloc(sizeof(Player));

    Thing thing = (Thing){
        .position = position,
        .rotation = rotation,
        .sin_rot  = sin(rotation),
        .cos_rot  = cos(rotation),
        .radius   = radius,
    };

    player->_ = (Actor){
        ._          = thing,
        .prev_pos   = position,
        .velocity   = Vector2Zero(),
        .speed      = 5.0f,
        .turn_speed = 0.0f,
        .prev_rot   = rotation,
        .angular_velocity = 0.0f,
    };

    player->controller = controller;

    return player;
}

void Player_update(Player *player, float delta){
    Actor *actor = &player->_;
    Thing *thing = &actor->_;
    
    float rotate = 0;

    Vector2 move;
    Vector2 offset;

    rotate = (
        (int)IsGamePadButtonPressed(
        player->controller,
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) 
        * actor->turn_speed) - (
        (int)IsGamePadButtonPressed(
        player->controller,
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT)
        * actor->turn_speed
    );

    actor->angular_velocity = rotate;
    Actor_rotate(actor, delta);
    
    move.x = (
        (int)IsGamePadButtonPressed(
        player->controller,
        GAMEPAD_BUTTON_LEFT_FACE_RIGHT)
        * actor->speed) - (
        (int)IsGamePadButtonPressed(
        player->controller,
        GAMEPAD_BUTTON_LEFT_FACE_LEFT)
        * actor->speed
    );
    move.y = (
        (int)IsGamePadButtonPressed(
        player->controller,
        GAMEPAD_BUTTON_LEFT_FACE_UP)
        * actor->speed) - (
        (int)IsGamePadButtonPressed(
        player->controller,
        GAMEPAD_BUTTON_LEFT_FACE_DOWN)
        * actor->speed
    );

    actor->velocity = Vector2Rotate(move,thing->rotation);

    Actor_move(actor, delta);
}

