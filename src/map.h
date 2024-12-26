#ifndef MAP_H
#define MAP_H

#include <raylib.h>

/* Uncomment the following #define to turn debug output on.
   It gets #define'd BEFORE "#include "defs.h". */
#define DEBUG

#include "defs.h"
#include "player.h"


typedef enum {
    EAST  = 0, /* +X */
    NORTH = 1, /* -Z */
    WEST  = 2, /* -X */
    SOUTH = 3, /* +Z */
} 
Cardinals;


/*Color 
WALL_COLORS[] = {
    RED,
    GREEN,
    BLUE,
    YELLOW
};*/

typedef enum {
    PORTAL,
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
    Index2D index;
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

Cell Cell_new();

Map *Map_new(const char *name, uint size, uint cell_width);
void Map_free(Map *map);

void Map_render(Map *map, Player *player);


#endif /* MAP_H */
