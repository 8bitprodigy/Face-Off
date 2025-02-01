#ifndef PROJECTILE_H
#define PROJECTILE_H
    
#include "actor.h"
#include "defs.h"


#define PROJECTILE( self ) ((Projectile*)self)

typedef struct Projectile Projectile;

/* Constructor */
void        Projectile_init(Projectile *projectile, Actor *master, Vector2 position, Vector2 direction, float max_distance, uint8 damage, float speed, float radius);
Projectile *Projectile_new(Actor *master, Vector2 position, Vector2 direction, float max_distance, uint8 damage, float speed, float radius);
/* Destructor */
void        Projectile_free(Projectile *projectile);

/* Projectile Methods */
void        Projectile_update(Actor *actor, GameState *game_state);

#endif /* PROJECTILE_H */
