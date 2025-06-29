#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"


#define PLAYER( self ) ((Player*)(self))

typedef struct Player Player;

/* Constructor */
void    Player_init(Player *player,   Vector2 position, float rotation, float radius,    int controller);
Player *Player_new( Vector2 position, float   rotation, float radius,   int   controller);
/* Destructor */
void    Player_free(Player *player);

/* Getters */
Camera  *Player_get_Camera(Player *player);
Vector2  Player_get_position(Player *player);
float    Player_get_half_fov(Player *player);
Player  *Player_get_prev(Player *player);

/* List Operations */
void     Player_insert(Player *player1, Player *player2);
void     Player_remove(Player *player);

/* Player Methods */
void     Player_update(Actor *actor, GameState *game_state);

#endif /* PLAYER_H */
