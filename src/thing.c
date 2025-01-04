#include <stdlib.h>
#include "thing.h"


Thing
Thing_new(Vector2 position, float rotation, float radius)
{
    Thing thing = (Thing){
        .visible  = true,
        .position = position,
        .rotation = rotation,
        .sin_rot  = sin(rotation),
        .cos_rot  = cos(rotation),
        .radius   = radius,
    };
    
    return thing;
} /* Thing_new */


Thing
*Thing_new_ptr(Vector2 position, float rotation, float radius)
{
    Thing *thing = malloc(sizeof(Thing));

    if (!thing) {
        ERR_OUT("Failed to allocate memory for Actor.");
        return NULL;
    }

    *thing = Thing_new(position, rotation, radius);

    thing->prev = thing;
    thing->next = thing;
    
    return thing;
} /* Thing_new_ptr */


void
Thing_free(Thing *thing)
{
    Thing_pop(thing);
    free(thing);
} /* Thing_free */


void
Thing_push(Thing *thing1, Thing *thing2)
{
    Thing *thing3 = thing1->prev;

    thing3->next = thing2;
    thing1->prev = thing2;
    
    thing2->next = thing1;
    thing2->prev = thing3;
} /* Thing_push */


void
Thing_pop(Thing *thing)
{
    Thing *thing1 = thing->prev;
    Thing *thing2 = thing->next;

    thing1->next = thing2;
    thing2->prev = thing1;
} /* Thing_pop */
