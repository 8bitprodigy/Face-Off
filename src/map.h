#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include <limits.h>
/* Uncomment the following #define to turn debug output on.
   It gets #define'd BEFORE "#include "defs.h". */
#define DEBUG

#include "defs.h"
#include "player.h"


#define DEBUG_CHECK_VIS
#define DEBUG_MAP_NEW

#define INDEX2D_NAN (Index2D){.x=INT_MAX,.y=INT_MAX}

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
        int   health;
        bool  locked;
        float timeout;
    };
} Wall;

typedef struct Cell Cell;

typedef struct
Index2D
{
    int x;
    int y;
} Index2D;

typedef struct
Cell
{
    Wall    walls[4];
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
    bool **render_buffer;
    uint cell_width;
} Map;

Cell Cell_new(void);

Map *Map_new(const char *name, uint size, uint cell_width);
void Map_free(Map *map);

void Map_render(Map *map, Player *player);
bool Map_check_collision(Map *map, Vector2 prev_pos, Vector2 new_pos, float radius, Vector2 *collision_point, Vector2 *collision_normal);


#endif /* MAP_H */
