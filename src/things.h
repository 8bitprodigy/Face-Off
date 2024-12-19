#ifndef ENTITIES_H
#define ENTITIES_H

#include <raylib.h>
#include <raymath.h>
#include "defs.h"

typedef struct 
Thing
{
    Vector2 position;
    float   rotation;
    float   sin_rot;
    float   cos_rot;
    float   radius;
    char    name[NAME_MAX_CHARS];
} Thing;

typedef struct 
Actor
{
    Thing   _;
    Vector2 prev_pos;
    Vector2 velocity;
    float   speed;
    float   turn_speed;
    float   prev_rot;
    float   angular_velocity;
    Mesh    *body;
    Color   color_body;
    Color   color_outline;
    
} Actor;

/* Actor Methods */
void Actor_rotate(Actor *actor, float delta);
void Actor_move(Actor *actor, float delta);

#endif /* ENTITIES_H */
