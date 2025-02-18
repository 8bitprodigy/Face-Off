#ifndef THING_PRIVATE_H
#define THING_PRIVATE_H

#include <raylib.h>
#include "thing.h"


typedef struct 
Thing
{
    struct Thing *prev;
    struct Thing *next;
    
    bool          visible;
    
    Vector2       position;
    
    float         rotation;
    float         sin_rot;
    float         cos_rot;
    
    float         radius;
    
    char         *name;
    
    Visualizer    body;
    Color         color_body;
    Color         color_outline;
} Thing;


#endif /* THING_PRIVATE_H */
