#include <stdlib.h>
#include "thing_private.h"
//#define DEBUG
#include "defs.h"


/****************************
    C O N S T R U C T O R    
****************************/
void
Thing_init(Thing *thing, Vector2 position, float rotation, float radius)
{
    if (!thing) return;

    // Initialize all fields
    thing->visible  = true;
    thing->position = position;
    thing->rotation = rotation;
    thing->sin_rot  = sin(rotation);
    thing->cos_rot  = cos(rotation);
    thing->radius   = radius;

    thing->prev = thing;
    thing->next = thing;

    thing->name = NULL;
    //thing->body = NULL;
} /* Thing_new */

Thing *
Thing_new(Vector2 position, float rotation, float radius)
{
    Thing *thing = malloc(sizeof(Thing));

    if (!thing) {
        ERR_OUT("Unable to allocate memory for Thing.");
        return NULL;
    }

    Thing_init(thing, position, rotation, radius);    
    return thing;
} /* Thing_new_ptr */


/**************************
    D E S T R U C T O R    
**************************/
void
Thing_free(Thing *thing)
{
    Thing_remove(thing);
    free(thing);
} /* Thing_free */


/********************
    G E T T E R S    
********************/
Thing *
Thing_get_prev(Thing *thing)
{
    return thing->prev;
} /* Thing_get_prev */

Thing *
Thing_get_next(Thing *thing)
{
    return thing->next;
} /* Thing_get_next */

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


/************************************
    L I S T   O P E R A T I O N S    
************************************/
/*        A D D    */
void
Thing_insert(Thing *thing1, Thing *thing2)
{
    Thing *thing3 = thing1->prev;

    thing3->next = thing2;
    thing1->prev = thing2;
    
    thing2->next = thing1;
    thing2->prev = thing3;
} /* Thing_push */

/*        R E M O V E    */
void
Thing_remove(Thing *thing)
{
    Thing *thing1 = thing->prev;
    Thing *thing2 = thing->next;

    thing1->next = thing2;
    thing2->prev = thing1;
} /* Thing_pop */

/********************************
    T H I N G   M E T H O D S 
********************************/
/*        D R A W    */
void
Thing_draw(Thing *self, Camera *camera)
{
    Visualizer *body = &self->body;

    DrawSphere(VECTOR2_TO_3(self->position, 0.5f), self->radius, self->color_body);
    /*
    if      (body->type == MESH) {
        DrawMesh(*body->data.mesh, LoadMaterialDefault(), MATRIX(VECTOR2_TO_3(self->position, 0.5f),self->rotation));
    }
    else if (body->type == SPRITE) {
        DrawBillboard(*camera, , VECTOR2_TO_3(self->position, 0.5f), 1.0f, self->color_body);
    }
    */
} /* Thing_draw */
