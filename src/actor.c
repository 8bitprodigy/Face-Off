#include <stdlib.h>
#include "actor.h"

Actor
*Actor_new(ActorType type)
{
    Thing thing;
    Actor *actor = malloc(sizeof(Actor));
    if (!actor) {
        ERR_OUT("Failed to allocate memory for Actor.");
        return NULL;
    }
    
    thing = (Thing){
        .position = Vector2Zero(),
        .rotation = 0.0f,
        .sin_rot  = sin(0.0f),
        .cos_rot  = cos(0.0f),
        .radius   = 0.5f,
    };
    
    actor->_                = thing;
    actor->prev_pos         = Vector2Zero();
    actor->velocity         = Vector2Zero();
    
    switch (type) {
    case PLAYER:
        actor->speed            = 5.0f;
        actor->turn_speed       = 1.0f;
        actor->prev_rot         = 0.0f;
        actor->angular_velocity = 0.0f;
        actor->health           = 3;
        break;
    case DERVISH:
        actor->speed            = 0.0f;
        actor->turn_speed       = 5.0f;
        actor->prev_rot         = 0.0f;
        actor->angular_velocity = 0.0f;
        actor->health           = 1;
        break;
    case ISOPOD:
        break;
    default:
        ERR_OUT("Actor type undefined!");
        return NULL;
    }
    
    actor->type = type;
    
    return actor;
}

void 
Actor_rotate(Actor *actor, float delta)
{
    Thing *thing = &actor->_;
    float rotation = thing->rotation;

    actor->prev_rot  = rotation;
    
    DBG_OUT("Rotation: %.4f", actor->angular_velocity);
    thing->rotation  = NORMALIZE_ANGLE(actor->angular_velocity * delta + rotation);
    //printf("Rotation: %.4f\n", thing->rotation);
    thing->sin_rot   = sin(rotation);
    thing->cos_rot   = cos(rotation);
} /* Actor_rotate */

void 
Actor_move(Actor *actor, float delta)
{
    Thing *thing         = &actor->_;
    Vector2 position     = thing->position;
    Vector2 new_position = Vector2Add(position, Vector2Scale(actor->velocity, delta));
    //printf("New Position>\t x: %.4f | y: %.4f\n", new_position.x, new_position.y); 
    
    /* Todo... */
    DBG_OUT("Actor position: { X: %.4f,\tY: %.4f }",position.x,position.y);
    actor->prev_pos = position;
    thing->position = new_position;
} /* Actor_move */
