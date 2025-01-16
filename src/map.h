#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include "gamestate.h"
/* Uncomment the following #define to turn debug output on.
   It gets #define'd BEFORE "#include "defs.h". */
//#define DEBUG
#include "defs.h"


#define DEBUG_CHECK_VIS
#define DEBUG_MAP_NEW

#define INDEX2D( _x_, _y_ ) ((Index2D){.x=_x_,.y=_y_})
#define INDEX2D_NAN (Index2D){.x=INT_MAX,.y=INT_MAX}


typedef enum {
    EAST  = 0, /* +X */
    NORTH = 1, /* -Z */
    WEST  = 2, /* -X */
    SOUTH = 3, /* +Z */
} 
Cardinals;

typedef enum {
    PORTAL,
    SOLID,
    BREAKABLE,
    LOCKED,
    TIMED,
    HIDDEN,
    ILLUSION,
    EXIT,
}
WallType;


typedef struct Wall Wall;

typedef struct Cell Cell;

typedef struct Map Map;

typedef struct
Index2D
{
    int x;
    int y;
} Index2D;


void     Cell_init(Cell *cell);

Map     *Map_new(const char *name, uint size, uint cell_width);
void     Map_free(Map *map);

Index2D  Map_get_index(Map *map, Vector2 position);

void     Map_render(Map *map, Player *player);
bool     Map_check_Actor_collision(Map *map, Actor *actor, Vector2 new_pos, Vector2 *collision_point, Vector2 *collision_normal);


#endif /* MAP_H */
