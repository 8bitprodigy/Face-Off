#include <stdlib.h>

#include "projectile_private.h"


Texture2D tex;

void
Projectile_init_texture()
{
    Image img = GenImageColor(64,64,WHITE);
    tex = LoadTextureFromImage(img);
    UnloadImage(img);
}

/****************************
    C O N S T R U C T O R   
****************************/
void
Projectile_init(Projectile *projectile, Actor *master, float max_distance, uint8 damage, float speed, Vector2 position, float rotation, float radius)
{
    Thing *thing             = THING(projectile);
    Actor *actor             = ACTOR(projectile);

    Actor_init(actor, BODY_SPRITE(&tex), position, rotation, radius);
    
    actor->speed             = speed;
    actor->health            = damage;
    actor->update            = &Projectile_update;
    
    projectile->master       = master;
    projectile->max_distance = max_distance;
    projectile->damage       = damage;
} /* Projectile_init */

Projectile *
Projectile_new(Actor *master, float max_distance, uint8 damage, float speed, Vector2 position, float rotation, float radius)
{
    DBG_OUT("Creating Projectile...");
    Projectile *projectile = malloc(sizeof(Projectile));
    if (!projectile){
        ERR_OUT("Failed to allocate memory for Projectile.");
        return NULL;
    }
    
    Projectile_init(projectile, master, max_distance, damage, speed, position, rotation, radius);
    
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
    Thing *thing = THING(actor);
    //DBG_OUT("Projectile updating! { X: %.4f\tY: %.4f }", thing->position.x, thing->position.y);
    Actor_move(actor, game_state);
} /* Projectile_update */

void
Projectile_hit(Actor *actor, Actor *collider, GameState *game_state)
{
    Projectile *self = PROJECTILE(actor);
    
    if (collider == self->master) return;
    
    Projectile_free(self);
} /* Projectile_hit */

void
Projectile_on_wall(Actor *actor, Vector2 position, Vector2 normal)
{
    Projectile *self = PROJECTILE(actor);
    Projectile_free(self);
} /* Projectile_on_wall */
