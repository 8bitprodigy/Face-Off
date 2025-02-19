#ifndef ACTOR_PRIVATE_H
#define ACTOR_PRIVATE_H

#include "actor.h"
#define DEBUG
#include "defs.h"
#include "thing_private.h"


typedef struct 
Actor
{
    Thing        base;
    
    struct Actor *prev;
    struct Actor *next;
    
    Vector2       prev_pos;
    Vector2       velocity;
    float         speed;
    
    float         turn_speed;
    float         prev_rot;
    float         angular_velocity;
    
    uint8         health;
    
    void          (*update )(struct Actor *, GameState *);
    void          (*on_hit )(struct Actor *, struct Actor *, GameState *);
    void          (*on_wall)(struct Actor *, Vector2,        Vector2);
} Actor;


void Actor_shoot(Actor *actor, GameState *game_state);

#endif /* ACTOR_PRIVATE_H */
