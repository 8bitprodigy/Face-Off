#ifndef THING_H
#define THING_H

#include <raylib.h>
#include <raymath.h>


typedef struct Thing Thing;

/* Constructors */
void  Thing_init(Thing *thing, Vector2 position, float rotation, float radius);
Thing *Thing_new(Vector2 position, float rotation, float radius);
/* Destructor */
void Thing_free(Thing *thing);

/* Getters */
Vector2 Thing_get_position(Thing *thing);
float   Thing_get_rotation(Thing *thing);

/* List Operations */
void Thing_push(Thing *thing1, Thing *thing2); /* pun intended */
void Thing_pop(Thing *thing);

/* Thing Methods */
void Thing_draw(Thing *thing);

#endif /* THING_H */

