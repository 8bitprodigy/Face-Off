#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include "defs.h"
#include "player.h"

typedef enum {
    EAST,  /* +X */
    NORTH, /* -Z */
    WEST,  /* -X */
    SOUTH, /* +Z */
    
} 
Cardinals;

const Color 
colors[] = {
    RED,
    GREEN,
    BLUE,
    YELLOW
};

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
    unsigned int x;
    unsigned int y;
} Index2D;

typedef struct
Cell
{
    Wall    walls[4];
    Cell    *neighbors[4];
    Vector2 corners[4];
    Vector2 center;
    Vector3 geometry[10];
} Cell;

typedef struct
Map
{
    char         name[MAP_NAME_MAX_CHARS];
    unsigned int size;
    Cell         (*cells)[];
    unsigned int cell_width;
} Map;

Map *Map_new(unsigned int size);
void Map_free(Map *map);

void Map_render(Player *player);

Index2D Map_get_index(Map *map, Vector2 position);

#endif /* MAP_H */
