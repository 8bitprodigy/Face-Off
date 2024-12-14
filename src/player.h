#ifndef PLAYER_H
#define PLAYER_H

#include "things.h"

typedef struct Player{
    Actor  _;
    Camera camera;
    int    controller;
} Player;

Player *Player_new(Vector2 position, float rotation, float radius, int controller);

void Player_update(Player *player, float delta);

#endif /* PLAYER_H */
