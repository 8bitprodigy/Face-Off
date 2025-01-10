#include <stdlib.h>
#include "thing_private.h"
#define DEBUG
#include "defs.h"


/******************************
*    C O N S T R U C T O R    *
******************************/
void
Thing_init(Thing *thing, Vector2 position, float rotation, float radius)
{
    *thing = (Thing){
        .prev     = thing,
        .next     = thing,
        
        .visible  = true,
        
        .position = position,
        
        .rotation = rotation,
        .sin_rot  = sin(rotation),
        .cos_rot  = cos(rotation),
        
        .radius   = radius,
    };
} /* Thing_new */

Thing
*Thing_new(Vector2 position, float rotation, float radius)
{
    Thing *thing = malloc(sizeof(Thing));

    if (!thing) {
        ERR_OUT("Failed to allocate memory for Actor.");
        return NULL;
    }

    Thing_init(thing, position, rotation, radius);

    thing->prev = thing;
    thing->next = thing;
    
    return thing;
} /* Thing_new_ptr */


/****************************
*    D E S T R U C T O R    *
****************************/
void
Thing_free(Thing *thing)
{
    Thing_pop(thing);
    free(thing);
} /* Thing_free */


/**********************
*    G E T T E R S    *
**********************/
Vector2
Thing_get_position(Thing *thing)
{
    return thing->position;
} /* Thing_get_position */

float
Thing_get_rotation(Thing *thing)
{
    return thing->rotation;
} /* Thing_get_rotation */


/**************************************
*    L I S T   O P E R A T I O N S    *
**************************************/
/*        A D D    */
void
Thing_push(Thing *thing1, Thing *thing2)
{
    Thing *thing3 = thing1->prev;

    thing3->next = thing2;
    thing1->prev = thing2;
    
    thing2->next = thing1;
    thing2->prev = thing3;
} /* Thing_push */

/*        R E M O V E    */
void
Thing_pop(Thing *thing)
{
    Thing *thing1 = thing->prev;
    Thing *thing2 = thing->next;

    thing1->next = thing2;
    thing2->prev = thing1;
} /* Thing_pop */
