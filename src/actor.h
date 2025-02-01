#ifndef ACTOR_H
#define ACTOR_H

#include <raylib.h>
#include <raymath.h>
#include "thing.h"


#define ACTOR( self ) ((Actor*)(self))

typedef struct GameState GameState;
typedef struct Map Map;

typedef struct Actor Actor;

/* Actor Constructor */
void  Actor_init(Actor *actor, Vector2 position, float rotation, float radius);
Actor *Actor_new(Vector2 position, float rotation, float radius);
/* Actor Destructor */
void Actor_free(Actor *actor);

/* Getters */
Actor   *Actor_get_prev(   Actor *actor);
Actor   *Actor_get_next(   Actor *actor);
Vector2 Actor_get_position(Actor *actor);
float   Actor_get_radius(  Actor *actor);

/* List Operations */
void Actor_insert(Actor *actor1, Actor *actor2);
void Actor_remove( Actor *actor);

/* Actor Methods */
void Actor_update(Actor *actor, GameState *game_state);
void Actor_draw(  Actor *actor);
void Actor_rotate(Actor *actor, GameState *game_state);
void Actor_move(  Actor *actor, GameState *game_state);

#endif /* ACTOR_H */
