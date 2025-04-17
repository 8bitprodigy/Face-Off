#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "map_private.h"


/****************************
    I N I T I A L I Z E R    
****************************/
void
Map_init(Map *map, char name[MAP_NAME_MAX_CHARS])
{
	if (!map) return;
	
	memcpy(map->name, name, MAP_NAME_MAX_CHARS);
	map->free                  = NULL;
	map->load                  = NULL;
	map->update                = NULL;
	map->render                = NULL;
	map->check_Actor_Collision = NULL;
} /* Map_init */

/****************************
    I N I T I A L I Z E R    
****************************/
void
Map_free(Map *self)
{
    if (!self->free) return;
    self->free(self);
} /* Map_free */


/***************************
    M A P  M E T H O D S    
***************************/

void
Map_update(Map *self, GameState *game_state)
{
    if (!self->update) return;
    self->update(self, game_state);
} /* Map_update */

void
Map_render(Map *self, Player *player)
{
    if (!self->render) return;
    self->render(self, player);
} /* Map_render */

bool
Map_check_Actor_collision(
	Map     *self,
	Actor   *actor, 
	Vector2  new_pos, 
	Vector2 *collision_point, 
	Vector2 *collision_normal
)
{
    if (!self->check_Actor_Collision) return false;
    return self->check_Actor_Collision(self, actor, new_pos, collision_point, collision_normal);
} /* Map_check_Actor_collision */
