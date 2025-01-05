#include <stdlib.h>
#include "face-off.h"
#include "actor.h"
#include "map.h"

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
    
    actor->type   = type;
    actor->update = &Actor_move;
    
    return actor;
}


void
Actor_free(Actor *actor)
{
    Thing_pop(&actor->_);
    Actor_pop(actor);
    free(actor);
} /* Actor_free */


void
Actor_push(Actor *actor1, Actor *actor2)
{
    Actor *actor3 = actor1->prev;

    actor3->next = actor2;
    actor1->prev = actor2;
    
    actor2->next = actor1;
    actor2->prev = actor3;

    //Thing_push(&actor1->_, &actor2->_);
} /* Actor_push */


void
Actor_pop(Actor *actor)
{
    Actor *actor1 = actor->prev;
    Actor *actor2 = actor->next;

    actor1->next = actor2;
    actor2->prev = actor1;

    //Thing_pop(&actor->_);
} /* Actor_pop */


void 
Actor_rotate(Actor *actor, float delta)
{
    Thing *thing   = &actor->_;
    float rotation = thing->rotation;

    actor->prev_rot  = rotation;
    
    DBG_OUT("Rotation: %.4f", actor->angular_velocity);
    thing->rotation  = NORMALIZE_ANGLE(actor->angular_velocity * delta + rotation);
    //printf("Rotation: %.4f\n", thing->rotation);
    thing->sin_rot   = sin(rotation);
    thing->cos_rot   = cos(rotation);
} /* Actor_rotate */

void 
Actor_move(Actor *actor, float delta, GameState *game_state)
{
    Thing   *thing       = &actor->_;
    Map     *map         = game_state->map;
    Vector2 position     = thing->position;
    Vector2 new_position = Vector2Add(position, Vector2Scale(actor->velocity, delta));
    Vector2 collision_point;
    Vector2 collision_normal;
    //printf("New Position>\t x: %.4f | y: %.4f\n", new_position.x, new_position.y); 
    if (Map_check_collision(map,position, new_position, thing->radius, &collision_point, &collision_normal)) {
        position = Vector2Add(
            collision_point, 
            Vector2Multiply(
                position, 
                (Vector2){
                    .x = fabsf(collision_normal.y),
                    .y = fabsf(collision_normal.x)
                }
            )
        );
    }
    /* Todo... */
    DBG_OUT("Actor position: { X: %.4f,\tY: %.4f }",position.x,position.y);
    actor->prev_pos = position;
    thing->position = new_position;
} /* Actor_move */
