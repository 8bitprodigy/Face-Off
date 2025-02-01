#include <stdlib.h>

#include "projectile_private.h"


/****************************
    C O N S T R U C T O R   
****************************/
void
Projectile_init(Projectile *projectile, Actor *master, Vector2 position, Vector2 direction, float max_distance, uint8 damage, float speed, float radius)
{
    Thing *thing             = THING(projectile);
    Actor *actor             = ACTOR(projectile);
    
    projectile->master       = master;
    projectile->max_distance = max_distance;
    
    actor->speed             = speed;
    actor->health            = damage;
    actor->velocity          = VECTOR2(direction.x * speed, direction.y * speed);
    
    thing->radius            = radius;
    thing->cos_rot           = direction.x;
    thing->sin_rot           = direction.y;
    thing->rotation          = atan2(direction.x, direction.y);
} /* Projectile_init */

Projectile *
Projectile_new(Actor *master, Vector2 position, Vector2 direction, float max_distance, uint8 damage, float speed, float radius)
{
    Projectile *projectile = malloc(sizeof(Projectile));
    if (!projectile){
        ERR_OUT("Failed to allocate memory for Projectile.");
        return NULL;
    }
    
    Projectile_new(master, position, direction, max_distance, damage, speed, radius);
    
    return projectile;
} /* Projectile_new */


/**************************
    D E S T R U C T O R    
**************************/
void
Projectile_free(Projectile *projectile)
{
    free(projectile);
} /* Projectile_free */


/*******************
   M E T H O D S
*******************/
void
Projectile_update(Actor *actor, GameState *game_state)
{
    Actor_move(actor, game_state);
} /* Projectile_update */

void
Projectile_hit(Actor *actor, Actor *collider, GameState *game_state)
{
    Projectile *self = PROJECTILE(actor);
    Projectile_free(self);
} /* Projectile_hit */

void
Projectile_on_wall(Actor *actor, Vector2 position, Vector2 normal)
{
    Projectile *self = PROJECTILE(actor);
    Projectile_free(self);
} /* Projectile_on_wall */
