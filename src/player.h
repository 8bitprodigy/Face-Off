#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"

typedef struct Player {
    Actor  _;

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

/* Constructor */
Player *Player_new(Vector2 position, float rotation, float radius, int controller);
/* Destructor */
void Player_free(Player *player);

/* List Operations */
void Player_push(Player *player1, Player *player2);
void Player_pop(Player *player);

/* Player Methods */
void Player_update(Player *player, float delta, GameState *game_state);

#endif /* PLAYER_H */
