#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"

typedef struct Player {
    Actor  *_;
    Camera camera;
    float  fov;
    float  half_fov;
    int    controller;
    int    score;
    int    team;
} Player;

/* Constructor */
Player *Player_new(Vector2 position, float rotation, float radius, int controller);
/* Destructor */
void Player_free(Player *player);

/* Player Methods */
void Player_update(Player *player, float delta);

#endif /* PLAYER_H */
