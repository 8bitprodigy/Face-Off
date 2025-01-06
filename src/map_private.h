#include "map.h"
//#define DEBUG
#include "defs.h"


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
    uint cell_width;
    bool **render_buffer;
} Map;
