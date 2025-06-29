#ifndef MAP_PRIVATE_H
#define MAP_PRIVATE_H

#include "actor.h"
#include "map.h"

typedef struct
Map
{
    char name[MAP_NAME_MAX_CHARS];
    void (*free                 )(struct Map *map);
    void (*load                 )(struct Map *map, GameState *game_state);
    void (*update               )(struct Map *map, GameState *game_state);
    void (*render               )(struct Map *map, Player    *player);
    bool (*check_Actor_Collision)(struct Map *map, Actor     *actor,       Vector2 new_pos, Vector2 *collision_point, Vector2 *collision_normal);
}
Map;


#endif /* MAP_PRIVATE_H */
