#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"

typedef struct Player Player;

/* Constructor */
void   Player_init(Player *player, Vector2 position, float rotation, float radius, int controller);
Player *Player_new(Vector2 position, float rotation, float radius, int controller);
/* Destructor */
void Player_free(Player *player);

/* Getters */
Actor   *Player_get_Actor(Player *player);
Camera  *Player_get_Camera(Player *player);
Vector2 Player_get_position(Player *player);
float   Player_get_half_fov(Player *player);

/* List Operations */
void Player_push(Player *player1, Player *player2);
void Player_pop(Player *player);

/* Player Methods */
void Player_update(Actor *actor, GameState *game_state);

#endif /* PLAYER_H */
