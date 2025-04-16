#ifndef MAP_H
#define MAP_H

#include "defs.h"
#include "gamestate.h"

 
#define MAP( self ) ((Map*)self)

typedef struct Map Map;


/* Map Initializer */
void     Map_init(  Map  *map,    char       name[MAP_NAME_MAX_CHARS]); 

/* Map Methods */
void     Map_update(Map  *map,    GameState *game_state);
void     Map_render(Map  *map,    Player    *player);
bool     Map_check_Actor_collision(
	Map     *map,
	Actor   *actor, 
	Vector2  new_pos, 
	Vector2 *collision_point, 
	Vector2 *collision_normal
);


#endif /* MAP_H */
