#ifndef PROJECTILE_H
#define PROJECTILE_H
    
#include "actor.h"
#define DEBUG
#include "defs.h"


#define PROJECTILE( self ) ((Projectile*)self)
#define PROJECTILE_MAX_DIST (1024.0f)

typedef struct Projectile Projectile;

/* Constructor */
void        Projectile_init(Projectile *projectile, Actor *master,       float max_distance, uint8 damage, float   speed,    Vector2 position,   float rotation, float radius);
Projectile *Projectile_new( Actor      *master,     float  max_distance, uint8 damage,       float speed,  Vector2 position, float   rotation,   float radius);
/* Destructor */
void        Projectile_free(Projectile *projectile);

/* Projectile Methods */
void        Projectile_update(Actor *actor, GameState *game_state);

void        Projectile_init_texture();

#endif /* PROJECTILE_H */
