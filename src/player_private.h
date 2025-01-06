#ifndef PLAYER_PRIVATE_H
#define PLAYER_PRIVATE_H

#include "player.h"
#include "actor_private.h"


typedef struct Player {
    Actor  base;

    struct Player *prev;
    struct Player *next;
    
    Camera camera;
    bool   remote;
    float  fov;
    float  half_fov;
    int    controller;
    int    score;
    int    team;
} Player;

#endif /* PLAYER_PRIVATE_H */
