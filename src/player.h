#define PLAYER_H
#ifndef PLAYER_H

#indlude "things.h"

typedef struct {
    Actor  _;
    Camera camera;
    int    controller;
} Player;

void Player_new();

void Player_update(Player *player, float delta);

#endif /* PLAYER_H */
