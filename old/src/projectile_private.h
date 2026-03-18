#ifndef PROJECTILE_PRIVATE_H
#define PROJECTILE_PRIVATE_H

#include "actor_private.h"
#include "projectile.h"

typedef struct
Projectile
{
    Actor base;
    Actor *master;
    float max_distance;
    uint8 damage;
}
Projectile;

void Projectile_collide(Actor *actor, Actor   *collider, GameState *game_state);
void Projectile_on_wall(Actor *actor, Vector2  position, Vector2    normal);

#endif  /* PROJECTILE_PRIVATE_H */
