#ifndef ENTITIES_H
#define ENTITIES_H

#include <raylib.h>

#define NAME_MAX_CHARS 8

typedef struct {
    Vector2 position;
    Vector2 prev_pos;
    float   rotation;
    float   sin_rotation;
    float   cos_rotation;
    float   radius;
    char    name[NAME_MAX_CHARS];
} Thing;

typedef struct {
    Thing   _;
    float   speed;
    float   turn_speed;
    Vector2 velocity;
    float   angular_velocity;
    Mesh    *body;
    Color   color_body;
    Color   color_outline;
    
} Actor;

#endif /* ENTITIES_H */
