#include <stdlib.h>

#include "actor_private.h"
//#define DEBUG
#include "defs.h"
#include "gamestate.h"
#include "map.h"
#include "projectile.h"


void
Actor_nop(Actor *self, ...) 
{}


/****************************
    C O N S T R U C T O R    
****************************/
void
Actor_init(
    Actor   *actor, 
    Body     body, 
    Vector2  position, 
    float    rotation, 
    float    radius
)
{
    Thing *thing = &actor->base;

    Mesh mesh = GenMeshSphere(radius,8,8);
    
    Thing_init(thing, body, position, rotation, radius);
    
    actor->prev = actor;
    actor->next = actor;
    
    actor->prev_pos         = Vector2Zero();
    actor->velocity         = Vector2Zero();
    
    actor->speed            = 5.0f;
    actor->turn_speed       = 1.0f;
    actor->prev_rot         = 0.0f;
    actor->angular_velocity = 0.0f;
    actor->health           = 3;
    actor->update           = &Actor_move;
    actor->collide          = NULL;
    actor->on_wall          = NULL;
    
} /* Actor_init */

Actor *
Actor_new(
    Body    body, 
    Vector2 position, 
    float   rotation, 
    float   radius
)
{
    Actor *actor = malloc(sizeof(Actor));
    if (!actor) {
        ERR_OUT("Failed to allocate memory for Actor.");
        return NULL;
    }
    
    Actor_init(actor, body, position, rotation, radius);
    
    return actor;
} /* Actor_new */


/**************************
    D E S T R U C T O R    
**************************/
void
Actor_free(Actor *actor)
{
    Thing_remove(THING(actor));
    Actor_remove(actor);
    free(actor);
} /* Actor_free */


/**********************
*    G E T T E R S    *
**********************/
Actor *
Actor_get_prev(Actor *actor)
{
    return actor->prev;
} /* Actor_get_prev */

Actor *
Actor_get_next(Actor *actor)
{
    return actor->next;
} /* Actor_get_next */

Vector2
Actor_get_position(Actor *actor)
{
    return actor->base.position;
} /* Actor_get_position */

float
Actor_get_radius(Actor *actor)
{
    return actor->base.radius;
} /* Actor_get_radius */


/************************************
    L I S T   O P E R A T I O N S    
************************************/
/*        A D D    */
void
Actor_insert(Actor *actor1, Actor *actor2)
{
    if (!actor1 || !actor2) {
        ERR_OUT("Actor_insert received NULL pointer");
        return;
    }

    if (!actor1->prev || !actor1->next) {
        ERR_OUT("Actor_insert received improperly initialized actor1");
        return;
    }
    Actor *actor3 = actor1->prev;

    actor3->next = actor2;
    actor1->prev = actor2;
    
    actor2->next = actor1;
    actor2->prev = actor3;

} /* Actor_insert */

/*        R E M O V E    */
void
Actor_remove(Actor *actor)
{
    Actor *actor1 = actor->prev;
    Actor *actor2 = actor->next;

    actor1->next = actor2;
    actor2->prev = actor1;

} /* Actor_remove */


/********************************
    A C T O R   M E T H O D S    
********************************/
/*        U P D A T E    */
void
Actor_update(Actor *self, GameState *game_state)
{
    if (!self->update) return;
    self->update(self, game_state);
} /* Actor_update */

void
Actor_collide(Actor *self, Actor *collider, GameState *game_state)
{
    if (!self->collide) return;
    self->collide(self, collider, game_state);
} /* Actor_collide */

void
Actor_on_wall(Actor *self, Vector2 position, Vector2 normal)
{
    if (!self->on_wall) return;
    self->on_wall(self, position, normal);
} /* Actor_on_wall */


void 
Actor_rotate(Actor *self, GameState *game_state)
{
    Thing *thing   = &self->base;
    float rotation = thing->rotation;
    float delta    = GameState_get_delta(game_state);

    self->prev_rot  = rotation;
    
    thing->rotation  = NORMALIZE_ANGLE(self->angular_velocity * delta + rotation);
    //printf("Rotation: %.4f\n", thing->rotation);
    thing->sin_rot   = sin(rotation);
    thing->cos_rot   = cos(rotation);
} /* Actor_rotate */

void 
Actor_move(Actor *self, Vector2 move_dir, GameState *game_state)
{
    Thing   *thing       = &self->base;
    Map     *map         = GameState_get_Map(game_state);
    float   delta        = GameState_get_delta(game_state);
    
    Vector2 position     = thing->position;
    float   radius       = thing->radius;
    Vector2 new_position = Vector2Add(position, Vector2Scale(move_dir, delta));
    Vector2 perpendicular_velocity;
    Vector2 correction_offset;
    Vector2 slide_velocity;
    Vector2 collision_point;
    Vector2 collision_normal;
    
    if (
        !Map_check_Actor_collision(
            map, self, 
            new_position, 
            &collision_point,
            &collision_normal
        )
    ) goto finalize_move;
    new_position = collision_point;
    
finalize_move:
    self->prev_pos  = position;
    thing->position = new_position;
    self->velocity  = move_dir;
} /* Actor_move */

void
Actor_shoot(Actor *self, GameState *game_state)
{
    DBG_OUT("`Actor_shoot()` entered...");
    
    Thing   *thing     = THING(self);
    Vector2  velocity  = self->velocity;
    Vector2  direction = VECTOR2( thing->cos_rot, thing->sin_rot );
    Vector2  position  = Vector2Add(thing->position, Vector2Scale(direction, thing->radius));
    
    Projectile *projectile = Projectile_new(
        self, 
        PROJECTILE_MAX_DIST, 
        1, 
        5.0f + (Vector2Length(velocity) * CLAMP(Vector2DotProduct(direction, velocity),0,1)), 
        position, 
        thing->rotation, 
        0.1f
    );
    
    DBG_OUT("Projectile created at address %p", projectile);
    
    GameState_add_Actor(
        game_state,
        ACTOR( projectile )
    );
} /* Actor_shoot */

