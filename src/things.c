#include "things.h"

void Actor_rotate(Actor *actor, float delta){
    Thing *thing = &actor->_;

    actor->prev_rot  = thing->rotation;
    
    thing->rotation += actor->angular_velocity * delta;
    thing->sin_rot   = sin(thing->rotation);
    thing->cos_rot   = cos(thing->rotation);
}

void Actor_move(Actor *actor, float delta){
    Thing *thing = &actor->_;
    Vector2 new_position = Vector2Add(thing->position, Vector2Scale(actor->velocity, delta));
    /* Todo... */
    actor->prev_pos = thing->position;
    thing->position = new_position;
}
