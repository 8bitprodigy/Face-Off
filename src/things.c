//#include <stdio.h>
#include "defs.h"
#include "things.h"

void 
Actor_rotate(Actor *actor, float delta)
{
    Thing *thing = &actor->_;

    actor->prev_rot  = thing->rotation;
    
    thing->rotation  = NORMALIZE(actor->angular_velocity * delta + thing->rotation);
    //printf("Rotation: %.4f\n", thing->rotation);
    thing->sin_rot   = sin(thing->rotation);
    thing->cos_rot   = cos(thing->rotation);
} /* Actor_rotate */

void 
Actor_move(Actor *actor, float delta)
{
    Thing *thing = &actor->_;
    Vector2 new_position = Vector2Add(thing->position, Vector2Scale(actor->velocity, delta));
    //printf("New Position>\t x: %.4f | y: %.4f\n", new_position.x, new_position.y); 
    
    /* Todo... */
    
    actor->prev_pos = thing->position;
    thing->position = new_position;
} /* Actor_move */
