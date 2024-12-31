#ifndef ACTOR_H
#define ACTOR_H

#include <raylib.h>
#include <raymath.h>
#include "thing.h"
//#define DEBUG
#include "defs.h"


typedef enum {
    PLAYER,
    DERVISH,
    ISOPOD,
}
ActorType;


typedef struct 
Actor
{
    Thing     _;
    ActorType type;
    Vector2   prev_pos;
    Vector2   velocity;
    float     speed;
    float     turn_speed;
    float     prev_rot;
    float     angular_velocity;
    int       health;
} Actor;

/* Actor Constructor */
Actor *Actor_new(ActorType type);
/* Actor Destructor */
void Actor_free(Actor *actor);

/* Actor Methods */
void Actor_update(Actor *actor, float delta);
void Actor_draw(Actor *actor);
void Actor_rotate(Actor *actor, float delta);
void Actor_move(Actor *actor, float delta);

#endif /* ACTOR_H */
