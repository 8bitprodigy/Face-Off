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

#endif  /* PROJECTILE_PRIVATE_H */
