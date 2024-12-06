#include <cmath.h>
#include "player.h"

void Player_update(Player *player, float delta){
    Actor *actor = player->_;
    Thing *thing = actor->_;
    
    float move_x = 0;
    float move_y = 0;
    float rotate = 0;

    rotate = (
        IsGamePadButtonPressed(
        player->controller,
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) 
        * actor->turn_speed) - (
        IsGamePadButtonPressed(
        player->controller,
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT)
        * actor->turn_speed
    );

    move_x = {
        IsGamePadButtonPressed(
        player->controller,
        GAMEPAD_BUTTON_LEFT_FACE_RIGHT)
        * actor->speed) - (
        IsGamePadButtonPressed(
        player->controller,
        GAMEPAD_BUTTON_LEFT_FACE_LEFT)
        * actor->speed
    );
    move_y = {
        IsGamePadButtonPressed(
        player->controller,
        GAMEPAD_BUTTON_LEFT_FACE_UP)
        * actor->speed) - (
        IsGamePadButtonPressed(
        player->controller,
        GAMEPAD_BUTTON_LEFT_FACE_DOWN)
        * actor->speed
    );
}

