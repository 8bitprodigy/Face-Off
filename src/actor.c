#include <stdlib.h>
#include "actor_private.h"
#include "gamestate.h"
#include "map.h"
//#define DEBUG
#include "defs.h"


/******************************
*    C O N S T R U C T O R    *
******************************/
void
Actor_init(Actor *actor, ActorType type, Vector2 position, float rotation, float radius)
{
    Thing *thing = &actor->base;

    actor->prev_pos         = Vector2Zero();
    actor->velocity         = Vector2Zero();
    
    switch (type) {
    case PLAYER:
        actor->speed            = 5.0f;
        actor->turn_speed       = 1.0f;
        actor->prev_rot         = 0.0f;
        actor->angular_velocity = 0.0f;
        actor->health           = 3;
        actor->update           = &Actor_move;
        Thing_init(thing, position, rotation, radius);
        break;
    case DERVISH:
        actor->speed            = 0.0f;
        actor->turn_speed       = 5.0f;
        actor->prev_rot         = 0.0f;
        actor->angular_velocity = 0.0f;
        actor->health           = 1;
        actor->update           = &Actor_update;
        Thing_init(thing, position, rotation, 0.5f);
        break;
    case ISOPOD:
        break;
    default:
        ERR_OUT("Actor type undefined!");
        return;
    }
    
    actor->type   = type;
}

Actor
*Actor_new(ActorType type, Vector2 position, float rotation, float radius)
{
    Actor *actor = malloc(sizeof(Actor));
    if (!actor) {
        ERR_OUT("Failed to allocate memory for Actor.");
        return NULL;
    }
    
    Actor_init(actor, type, position, rotation, radius);
    
    return actor;
}


/****************************
*    D E S T R U C T O R    *
****************************/
void
Actor_free(Actor *actor)
{
    Thing_pop(&actor->base);
    Actor_pop(actor);
    free(actor);
} /* Actor_free */


/**********************
*    G E T T E R S    *
**********************/
Thing
*Actor_get_Thing(Actor *actor)
{
    return &actor->base;
}

Vector2
Actor_get_position(Actor *actor)
{
    return actor->base.position;
}

float
Actor_get_radius(Actor *actor)
{
    return actor->base.radius;
}


/**************************************
*    L I S T   O P E R A T I O N S    *
**************************************/
/*        A D D    */
void
Actor_push(Actor *actor1, Actor *actor2)
{
    Actor *actor3 = actor1->prev;

    actor3->next = actor2;
    actor1->prev = actor2;
    
    actor2->next = actor1;
    actor2->prev = actor3;

} /* Actor_push */

/*        R E M O V E    */
void
Actor_pop(Actor *actor)
{
    Actor *actor1 = actor->prev;
    Actor *actor2 = actor->next;

    actor1->next = actor2;
    actor2->prev = actor1;

} /* Actor_pop */


/**********************************
*    A C T O R   M E T H O D S    *
**********************************/
/*        U P D A T E    */
void
Actor_update(Actor *actor, GameState *game_state)
{}

void 
Actor_rotate(Actor *actor, GameState *game_state)
{
    Thing *thing   = &actor->base;
    float rotation = thing->rotation;
    float delta    = GameState_get_delta(game_state);

    actor->prev_rot  = rotation;
    
    DBG_OUT("Rotation: %.4f", actor->angular_velocity);
    thing->rotation  = NORMALIZE_ANGLE(actor->angular_velocity * delta + rotation);
    //printf("Rotation: %.4f\n", thing->rotation);
    thing->sin_rot   = sin(rotation);
    thing->cos_rot   = cos(rotation);
} /* Actor_rotate */

void 
Actor_move(Actor *actor, GameState *game_state)
{
    Thing   *thing       = &actor->base;
    Map     *map         = GameState_get_Map(game_state);
    float   delta        = GameState_get_delta(game_state);
    
    Vector2 position     = thing->position;
    Vector2 new_position = Vector2Add(position, Vector2Scale(actor->velocity, delta));
    Vector2 collision_point;
    Vector2 collision_normal;
    
    //printf("New Position>\t x: %.4f | y: %.4f\n", new_position.x, new_position.y); 
    if (Map_check_Actor_collision(map, actor, new_position, &collision_point, &collision_normal)) {
        DBG_OUT("Wall collision!");
        new_position = Vector2Add(
            collision_point,
            /* This should nullify the part of the coord that would find itself inside the wall
            allowing the actor to slide along it. */
            Vector2Multiply(
                new_position, 
                VECTOR2(collision_normal.x, collision_normal.y)
            )
        );
        DBG_OUT("New Position: { X:%.4f |\tY:%.4f }", new_position.x, new_position.y);
    }
    /* Todo... */
    DBG_OUT("Actor position: { X: %.4f,\tY: %.4f }",position.x,position.y);
    actor->prev_pos = position;
    thing->position = new_position;
} /* Actor_move */
