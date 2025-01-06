#ifndef ACTOR_PRIVATE_H
#define ACTOR_PRIVATE_H

#include "actor.h"
#include "thing_private.h"


typedef struct 
Actor
{
    Thing        base;
    
    struct Actor *prev;
    struct Actor *next;
    
    ActorType   type;
    
    Vector2     prev_pos;
    Vector2     velocity;
    float       speed;
    
    float       turn_speed;
    float       prev_rot;
    float       angular_velocity;
    
    int         health;
    
    void        (*update)(struct Actor *, float, GameState *);
} Actor;


#endif /* ACTOR_PRIVATE_H */
