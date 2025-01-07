#ifndef ACTOR_H
#define ACTOR_H

#include <raylib.h>
#include <raymath.h>
#include "thing.h"

typedef struct GameState GameState;
typedef struct Map Map;

typedef enum {
    PLAYER,
    DERVISH,
    ISOPOD,
}
ActorType;


typedef struct Actor Actor;

/* Actor Constructor */
void  Actor_init(Actor *actor, ActorType type);
Actor *Actor_new(ActorType type);
/* Actor Destructor */
void Actor_free(Actor *actor);

/* Getters */
Thing   *Actor_get_Thing(  Actor *actor);
Vector2 Actor_get_position(Actor *actor);
float   Actor_get_radius(  Actor *actor);

/* List Operations */
void Actor_push(Actor *actor1, Actor *actor2);
void Actor_pop( Actor *actor);

/* Actor Methods */
void Actor_update(Actor *actor, GameState *game_state);
void Actor_draw(  Actor *actor);
void Actor_rotate(Actor *actor, GameState *game_state);
void Actor_move(  Actor *actor, GameState *game_state);

#endif /* ACTOR_H */
