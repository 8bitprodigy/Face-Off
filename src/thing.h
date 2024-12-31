#ifndef ENTITIES_H
#define ENTITIES_H

#include <raylib.h>
#include <raymath.h>
//#define DEBUG
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
    Mesh    *body;
    Color   color_body;
    Color   color_outline;
} Thing;

#endif /* ENTITIES_H */
