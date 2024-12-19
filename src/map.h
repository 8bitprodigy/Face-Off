#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include "defs.h"

#define Map_Get_Index( map, position ) \
    (Index2D) { \
        (int)((float)map->size / 2 + position.x / (float)map->cell_width), \
        (int)((float)map->size / 2 + position.y / (float)map->cell_width)  \
    }

typedef enum {
    EAST,  /* +X */
    NORTH, /* -Z */
    WEST,  /* -X */
    SOUTH, /* +Z */
    
} 
Cardinals;

/*const Color 
colors[] = {
    RED,
    GREEN,
    BLUE,
    YELLOW
};*/

typedef enum {
    NONE,
    SOLID,
    BREAKABLE,
    LOCKED,
    TIMED,
    HIDDEN,
    ILLUSION,
    EXIT
}
WallType;

typedef struct
Wall
{
    Color    color;
    WallType type;
    union {
        float health;
        bool  locked;
        float timeout;
    };
} Wall;

typedef struct Cell Cell;

typedef struct
Index2D
{
    uint x;
    uint y;
} Index2D;

typedef struct
Cell
{
    Wall    walls[4];
    Cell    *neighbors[4];
    Vector2 corners[4];
    Vector2 center;
} Cell;

typedef struct
Map
{
    char name[MAP_NAME_MAX_CHARS];
    uint size;
    Cell **cells;
    uint cell_width;
} Map;

Map *Map_new(const char *name, uint size, uint cell_width);
void Map_free(Map *map);

void Map_render(Map *map, Player *player);


#endif /* MAP_H */
