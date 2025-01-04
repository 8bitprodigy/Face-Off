#ifndef ENTITIES_H
#define ENTITIES_H

#include <raylib.h>
#include <raymath.h>
//#define DEBUG
#include "defs.h"


typedef struct 
Thing
{
    struct Thing *prev;
    struct Thing *next;
    
    bool         visible;
    
    Vector2      position;
    
    float        rotation;
    float        sin_rot;
    float        cos_rot;
    
    float        radius;
    
    char         *name;
    
    Mesh         *body;
    Color        color_body;
    Color        color_outline;
} Thing;

/* Constructors */
Thing Thing_new(Vector2 position, float rotation, float radius);
Thing *Thing_new_ptr(Vector2 position, float rotation, float radius);
/* Destructor */
void Thing_free(Thing *thing);

/* List Operations */
void Thing_push(Thing *thing1, Thing *thing2); /* pun intended */
void Thing_pop(Thing *thing);

/* Thing Methods */
void Thing_draw(Thing *thing);

#endif /* ENTITIES_H */
