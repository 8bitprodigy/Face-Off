#include <stdlib.h>
#include <limits.h>
#include <raylib.h>
#include <string.h>
#include <float.h>
#include "map.h"

/*    L O C A L   D E F I N E S    */
#define GET_FRUSTUM_EDGE( Position, Point )  (Vector2Add(Vector2Scale(Vector2Normalize(Vector2Subtract(Point, Position)),MAX_DRAW_DISTANCE),Position))


/****************************
*    L O C A L   D A T A    *
****************************/
float 
Wall_Normals[4] 
= {
    PI,
    HALF_PI,
    0,
    PI + HALF_PI
};

Vector2 
Wall_Vec2_Normals[4] 
= {
    VECTOR2(-1.0f,  0.0f),
    VECTOR2( 0.0f,  1.0f),
    VECTOR2( 1.0f,  0.0f),
    VECTOR2( 0.0f, -1.0f),
};

Color
Wall_Colors[4] 
= {
    RED,
    GREEN,
    BLUE,
    YELLOW,
};

Index2D 
Cell_Directions[4] 
= {
    INDEX2D( 1,  0),
    INDEX2D( 0, -1),
    INDEX2D(-1,  0),
    INDEX2D( 0,  1),
};


/********************************************
*    S T R U C T   D E F I N I T I O N S    *
********************************************/
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

/****************************************
*    H E L P E R   F U N C T I O N S    *
****************************************/
static inline Index2D
Index2D_add(Index2D index_1, Index2D index_2)
{
    return (Index2D){ .x = index_1.x + index_2.x, .y = index_1.y + index_2.y };
}

static inline bool
Index2D_equals(Index2D index1, Index2D index2)
{
    return index1.x == index2.x && index1.y == index2.y;
} /* Index2D_equals */

static inline bool
Index2D_OOB(Index2D index, uint size){
    return !(0 <= index.x && index.x < (int)size && 0 <= index.y && index.y < (int)size);
} /* Index2D_OOB */

static inline Vector2 
get_frustum_edge(Vector2 position, Vector2 point) 
{
    Vector2 direction;
    
    if (Vector2Distance(position, point) < FLT_EPSILON) {
        return position; 
    }
    direction = Vector2Normalize(Vector2Subtract(point, position));
    return Vector2Add(Vector2Scale(direction, MAX_DRAW_DISTANCE), position);
} /* get_frustum_edge */


/********************************
*    W A L L   M E T H O D S    *
********************************/
bool
Wall_check_frustum_intersection(Vector2 wall_start, Vector2 wall_end, Vector2 position, Vector2 l_frust, Vector2 r_frust, Vector2 *inside2, Vector2 *inside1)
{
    bool is_in_triangle = false;
    Vector2 dummy;
    
    //*inside1 = VECTOR2_NAN;
    //*inside2 = VECTOR2_NAN;

    //DBG_OUT("Wall Start: { X: %.4f,\tY: %.4f }", wall_start.x, wall_start.y);
    //DBG_OUT("Wall End:   { X: %.4f,\tY: %.4f }", wall_end.x,   wall_end.y);
    if (CheckCollisionPointTriangle(wall_end, position, r_frust, l_frust)) {
        *inside1       = wall_end;
        is_in_triangle = true;
    }
    if (CheckCollisionPointTriangle(wall_start, position, r_frust, l_frust)) {
        *inside2       = wall_start;
        is_in_triangle = true;
    }
    if (is_in_triangle) {
        //DBG_LINE(wall_start, wall_end, 0.1f, MAGENTA);
        return true;
    }
    
    if (CheckCollisionLines(wall_start, wall_end, position, r_frust, &dummy) ||
        CheckCollisionLines(wall_start, wall_end, position, l_frust, &dummy)) {
        //DBG_LINE(wall_start, wall_end, 0.1f, VIOLET);
        return true;
    }
    
    return false;
} /*Wall_check_frustum_intersection */


/********************************
*    C E L L   M E T H O D S    *
********************************/
/*        I N I T I A L I Z E R    */
void
Cell_init(Cell *cell)
{
    int  i;
    
    for ( i = 0; i < 4; i++ ) {
        cell->walls[i] = (Wall){
            .color  = Wall_Colors[i],
            .type   = PORTAL,
            .health = 0.0f
        };
        cell->corners[i] = Vector2Zero();
    }
    cell->center = Vector2Zero();
} /* Cell_new */


void
Cell_render(Cell *cell, uint cell_width)
{
    if (!cell) return;

    Vector2 *corners    = cell->corners;
    
    Wall *wall_east     = &cell->walls[EAST];
    Wall *wall_north    = &cell->walls[NORTH];
    Wall *wall_west     = &cell->walls[WEST];
    Wall *wall_south    = &cell->walls[SOUTH];

    Vector2 se_corner   = corners[0];
    Vector2 ne_corner   = corners[1];
    Vector2 nw_corner   = corners[2];
    Vector2 sw_corner   = corners[3];

    Vector3 pts_east[]  = {
        VECTOR2_TO_3(se_corner,0.0f),
        VECTOR2_TO_3(se_corner,1.0f),
        VECTOR2_TO_3(ne_corner,0.0f),
        VECTOR2_TO_3(ne_corner,1.0f),
    };
    Vector3 pts_north[] = {
        VECTOR2_TO_3(ne_corner,0.0f),
        VECTOR2_TO_3(ne_corner,1.0f),
        VECTOR2_TO_3(nw_corner,0.0f),
        VECTOR2_TO_3(nw_corner,1.0f),
    };
    Vector3 pts_west[]  = {
        VECTOR2_TO_3(nw_corner,0.0f),
        VECTOR2_TO_3(nw_corner,1.0f),
        VECTOR2_TO_3(sw_corner,0.0f),
        VECTOR2_TO_3(sw_corner,1.0f),
    };
    Vector3 pts_south[] = {
        VECTOR2_TO_3(sw_corner,0.0f),
        VECTOR2_TO_3(sw_corner,1.0f),
        VECTOR2_TO_3(se_corner,0.0f),
        VECTOR2_TO_3(se_corner,1.0f),
    };
    
    DrawPlane(
        VECTOR2_TO_3(cell->center, FLOOR_HEIGHT), 
        (Vector2){cell_width,cell_width},
        GRAY
    );
    if (wall_east->type)  DrawTriangleStrip3D( pts_east,  4, RED);
    if (wall_north->type) DrawTriangleStrip3D( pts_north, 4, GREEN);
    if (wall_west->type)  DrawTriangleStrip3D( pts_west,  4, BLUE);
    if (wall_south->type) DrawTriangleStrip3D( pts_south, 4, YELLOW);

#ifdef DEBUG_
    DrawCube(
        VECTOR2_TO_3(cell->corners[0], FLOOR_HEIGHT),
        0.1f, 0.1f, 0.1f, RED
    );
    DrawCube(
        VECTOR2_TO_3(cell->corners[1], FLOOR_HEIGHT),
        0.1f, 0.1f, 0.1f, GREEN
    );
    DrawCube(
        VECTOR2_TO_3(cell->corners[2], FLOOR_HEIGHT),
        0.1f, 0.1f, 0.1f, BLUE
    );
    DrawCube(
        VECTOR2_TO_3(cell->corners[3], FLOOR_HEIGHT),
        0.1f, 0.1f, 0.1f, YELLOW
    );
#endif /* DEBUG */
} /* Cell_render */


/**************************************
*    M A P   C O N S T R U C T O R    *
**************************************/
Map *
Map_new(const char *name, uint size, uint cell_width)
{
    int    i, j, w;
    
    Map     *map   = malloc(sizeof(Map));
    Cell    *cell;
    Wall    *walls;
    
    float   map_width       = size * cell_width;
    float   half_map_width  = map_width / 2.0f;
    float   half_cell_width = cell_width / 2.0f;
    //float   half_size       = size / 2.0f;
    Vector2 center;

    if (!map) {
        ERR_OUT("Failed to allocate memory for Map");
        return NULL;
    }
    
    strncpy(map->name, name, MAP_NAME_MAX_CHARS-1);
    map->name[MAP_NAME_MAX_CHARS-1] = '\0';
    map->size  = size;
    map->cells = malloc(size * sizeof(Cell *));
    if (!map->cells) {
        ERR_OUT("Failed to allocate memory for Map Cells.");
        free(map);
        return NULL;
    }
    for ( i = 0; i < (int)size; i++ ) {
        map->cells[i] = malloc(size * sizeof(Cell));
        if (!map->cells[i]) {
            ERR_OUT("Failed to allocate memory for Map Cell Array.");
            Map_free(map);
            return NULL;
        }
    }
    map->cell_width = cell_width;
    
    for ( i = 0; i < (int)size; i++ ) {
        for ( j = 0; j < (int)size; j++ ) {
            Cell_init(&map->cells[i][j] );
            cell  = &map->cells[i][j];
            walls = cell->walls;

            cell->index = (Index2D){i,j};
            center = (Vector2){
                i * cell_width + half_cell_width - half_map_width,
                j * cell_width + half_cell_width - half_map_width
            };
            cell->center     = center;

            cell->corners[0] = (Vector2) { /* SOUTHEAST */
                center.x + half_cell_width,
                center.y + half_cell_width
            };
            cell->corners[1] = (Vector2) { /* NORTHEAST */
                center.x + half_cell_width,
                center.y - half_cell_width
            };
            cell->corners[2] = (Vector2) { /* NORTHWEST */
                center.x - half_cell_width,
                center.y - half_cell_width
            };
            cell->corners[3] = (Vector2) { /* SOUTHWEST */
                center.x - half_cell_width,
                center.y + half_cell_width
            };

            if (!i) {
                walls[WEST].type   = SOLID;
                walls[WEST].color  = BLUE;
            } else if (i==(int)size-1) {
                walls[EAST].type   = SOLID;
                walls[EAST].color  = RED;
            } else {
                w = (uint)rand() >> 30;
                walls[WEST].type = w;
                walls[WEST].color = BLUE;
                map->cells[i-1][j].walls[EAST].type = w;
                map->cells[i-1][j].walls[EAST].color = RED;
            } 
            
            if (!j) {
                walls[NORTH].type  = SOLID;
                walls[NORTH].color = GREEN;
            } else if (j==(int)size-1) {
                walls[SOUTH].type  = SOLID;
                walls[SOUTH].color = YELLOW;
            } else {
                w = (uint)rand() >> 30;
                walls[NORTH].type = w;
                walls[NORTH].color = GREEN;
                map->cells[i][j-1].walls[SOUTH].type = w;
                map->cells[i][j-1].walls[SOUTH].color = RED;
            } 
        }
    }

    map->render_buffer = malloc(size * size * sizeof(bool));
    
    for (i = 0; i < (int)size; i++) {
        map->render_buffer[i] = (bool*)malloc(size*sizeof(bool));
        if (!map->render_buffer[i]) {
            ERR_OUT("Could not initialize render buffer inner array.");
            for(; 0 <= i; i--) {
                free(map->render_buffer[i]);
            }
            free(map->render_buffer);
            return NULL;
        }
    }
    
    return map;
} /* Map_new */


/************************************
*    M A P   D E S T R U C T O R    *
************************************/
void
Map_free(Map *map)
{
    uint i;
    if (!map) return;
    for ( i = 0; i < map->size; i++ ) {
        free(map->cells[i]);
    }
    free(map->cells);
    free(map);
} /* Map_free */


/******************************
*    M A P   M E T H O D S    *
******************************/
inline Index2D
Map_get_index(Map *map, Vector2 position) 
{
    return (Index2D) {
        (int)((map->size * map->cell_width / 2.0f + position.x) / (float)map->cell_width),
        (int)((map->size * map->cell_width / 2.0f + position.y) / (float)map->cell_width) 
    };
}

void
get_nearest_adjacent_cells(Map *map, Index2D index, Vector2 position, Cell **check_cells)
{
    int       i;
    
    int       size  = map->size; 
    Cell    **cells = map->cells;
    Index2D   offset;
    
    /* First checked cell should ALWAYS be the one the actor's coordinates are actually in. */
    check_cells[0] = &cells[index.x][index.y];
    
    if (!( /* This is readable. I don't exactly like it, but it's readable. */
        IS_IN_BOUNDS(index.x, 0, (int)map->size-1)
        && IS_IN_BOUNDS(index.y, 0, (int)size-1)
    )) return;
    
    offset.x = SIGN_BETWEEN(position.x, check_cells[0]->center.x) + index.x;
    offset.y = SIGN_BETWEEN(position.y, check_cells[0]->center.y) + index.y;
    /* Then we get adjacent cells(if applicable). */
    if (IS_IN_BOUNDS(offset.x, 0, size-1)) check_cells[1] = &cells[offset.x][index.y];
    else check_cells[1] = check_cells[0];
    if (IS_IN_BOUNDS(offset.y, 0, size-1)) check_cells[2] = &cells[index.x][offset.y];
    else check_cells[2] = check_cells[0];
    if (IS_IN_BOUNDS(offset.x, 0, size-1) && IS_IN_BOUNDS(offset.y, 0, size-1))
        check_cells[3] = &cells[offset.x][offset.y];
    else check_cells[3] = check_cells[0];
}

bool
is_corner_on_wall(Map *map, Index2D index, int corner)
{
    int   i;
    Cell *cells[4];
    Cell *cell     = &map->cells[index.x][index.y]; 
    
    get_nearest_adjacent_cells(map, index, cell->corners[corner], cells);
    
    for (i = 0; i < 4; i++) {
        if (!cells[i]) return false;
        if (cells[i]->walls[corner].type || cells[i]->walls[corner-1%4].type) return true;
        corner++;
    }
    
    return false;
}

void 
check_vis(Map *map, Index2D index, Thing *thing, Vector2 l_frustum, Vector2 r_frustum)
{
    uint    i, next, prev;
    
    Cell    *cell           = &map->cells[index.x][index.y];
    bool    **render_buffer = map->render_buffer;
    Vector2 *corners        = cell->corners;
    //Vector2 center          = cell->center;
    Wall    *walls          = cell->walls;
    bool    is_visible      = false;

    Vector2 position   = Thing_get_position(thing);
    //float   rotation   = thing->rotation;

    Vector2 inside_l;
    Vector2 inside_r;
    Index2D neighbor;

    //DBG_OUT("\n\tChecking cell: { X: %u,\tY: %u }",index.x,index.y);
    //if (!cell) return;
    if (render_buffer[index.x][index.y]) return;
    render_buffer[index.x][index.y] = true;
    //DBG_OUT("Cell Index: { X: %u,\tY: %u }", cell->index.x, cell->index.y);
    //Cell_render(cell,4);

    for (i = 0; i < 4; i++) {
        neighbor = Index2D_add(index, Cell_Directions[i]);
        next     = (i+1)%4;
        prev     = (i-1)%4;
        
        
        if ( Index2D_OOB(neighbor, map->size) ) {//|| (0 < DOT(rotation, Wall_Normals[i])) ) {
            //DBG_LINE(thing->position, cell->corners[i], 0.1f, YELLOW);
            continue;
        }
        //DBG_OUT("i: %u\ti+1%%4 : %u\t(i+1)%%4: %u", i, i+1%4, (i+1)%4);
        inside_l = l_frustum;
        inside_r = r_frustum;
        if (
            Wall_check_frustum_intersection(
                corners[i], 
                corners[next], 
                position, 
                l_frustum, 
                r_frustum, 
                &inside_l,
                &inside_r
            )
            //|| CheckCollisionPointTriangle(cell->center, position, r_frustum, l_frustum)
        ) {
            is_visible = true;
            if (walls[i].type == PORTAL) {
                //DBG_LINE(corners[i], corners[next], 0.1f, MAGENTA);
            
                //DBG_OUT("Wall %u is visible.",i);
                //DBG_OUT("East Inside: { X: %.4f,\tY: %.4f }\n", inside_r.x, inside_r.y );
                if (!Vector2Equals(inside_l, l_frustum) && !walls[prev].type) {
                    inside_l = GET_FRUSTUM_EDGE(position, inside_l);
                } else inside_l = l_frustum;
                if (!Vector2Equals(inside_r, r_frustum) && !walls[next].type) {
                    inside_r = GET_FRUSTUM_EDGE(position, inside_r);
                } else inside_r = r_frustum;
                if (walls[prev].type == PORTAL) inside_l = l_frustum;
                if (walls[next].type == PORTAL) inside_r = r_frustum;
                //inside_r = r_frustum; inside_l = l_frustum;
                //DBG_LINE(position, inside_l,0.2f, ORANGE);
                //DBG_LINE(position, inside_r,0.1f, GREEN);
                check_vis(map, neighbor, thing, inside_l, inside_r);
            }
            else {
                //DBG_OUT("Wall %u is NOT visible.",i);
                //DBG_LINE(position, corners[i],0.2f, RED);
            }
        }
    }
    if (is_visible) { // && !render_buffer[index.x][index.y]) {
        Cell_render(cell,4);
    } //else DBG_LINE(position, center, 0.3, PURPLE);
    //render_buffer[index.x][index.y] = true;
    //DBG_OUT("Cell checked.\n");
} /* Cell_check_vis */


void
Map_render(Map *map, Player *player)
{
    int    i, j;
    Thing   *thing            = THING(player);

    int    size               = map->size;
    bool    **render_buffer   = map->render_buffer;
    Index2D index             = Map_get_index(map, Thing_get_position(thing));
    //DBG_EXPR(Vector2 position = Thing_get_position(thing));
    float   r_fov_edge        = NORMALIZE_ANGLE(Thing_get_rotation(thing) + Player_get_half_fov(player));
    float   l_fov_edge        = NORMALIZE_ANGLE(Thing_get_rotation(thing) - Player_get_half_fov(player));
    Vector2 r_frustum         = Vector2Add(
        Thing_get_position(thing), 
        Vector2Scale(ANGLE_TO_VECTOR2(r_fov_edge), MAX_DRAW_DISTANCE)
    );
    Vector2 l_frustum         = Vector2Add(
        Thing_get_position(thing), 
        Vector2Scale(ANGLE_TO_VECTOR2(l_fov_edge), MAX_DRAW_DISTANCE)
    );

    if (!render_buffer) {
        ERR_OUT("Could not initialize render buffer.");
        return;
    }
    
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            render_buffer[i][j] = false;
        }
    }
    //DrawTriangle3D(VECTOR2_TO_3(r_frustum,0.1f),VECTOR2_TO_3(thing->position,0.1f),VECTOR2_TO_3(l_frustum,0.1f), BLUE);

    //DBG_LINE(r_frustum, Thing_get_position(thing), 0.1f, BLUE);
    //DBG_LINE(l_frustum, Thing_get_position(thing), 0.1f, BLUE);
    
    if (size <= index.x || size <= index.y) return;
    //DBG_OUT("Index : { X: %u, \tY: %u }\n",index.x,index.y);
    check_vis(map, index, thing, l_frustum, r_frustum);
    /*DBG_OUT("Buffer size: %u",buffer_size);
    for (i=0; i < buffer_size; i++) {
        printf("%u, ",i);
        Cell_render(&map->cells[render_buffer[i].x][render_buffer[i].y], map->cell_width);
    }*/
    //DBG_OUT("frame over.\n");
    
} /* Map_render */


bool
Map_check_Actor_collision(Map *map, Actor *actor, Vector2 new_pos, Vector2 *collision_point, Vector2 *collision_normal)
{
    int   cell_index, wall_index, next;

    int   size = map->size; 
    
    Cell  **cells = map->cells;
    Cell  *check_cells[4];
    Cell  *cell;
    Wall  *walls;

    Vector2 prev_pos = Actor_get_position(actor);
    float   radius   = Actor_get_radius(actor);

    Vector2 collision       = VECTOR2_NAN;
    Vector2 final_collision = new_pos;
    Vector2 *corners;
    Vector2 wall_start;
    Vector2 wall_end;
    
    Index2D offset;
    Index2D map_index = Map_get_index(map, prev_pos);

    if (!(IS_IN_BOUNDS(map_index.x, 0, (int)size-1)&&IS_IN_BOUNDS(map_index.y, 0, (int)size-1))) return false;
    /* First checked cell should ALWAYS be the one the actor's coordinates are actually in. */
    check_cells[0] = &cells[map_index.x][map_index.y];
    
    offset.x = SIGN_BETWEEN(prev_pos.x, check_cells[0]->center.x) + map_index.x;
    offset.y = SIGN_BETWEEN(prev_pos.y, check_cells[0]->center.y) + map_index.y;
    /* Then we get adjacent cells(if applicable). */
    if (IS_IN_BOUNDS(offset.x, 0, size-1)) check_cells[1] = &cells[offset.x][map_index.y];
    else check_cells[1] = check_cells[0];
    if (IS_IN_BOUNDS(offset.y, 0, size-1)) check_cells[2] = &cells[map_index.x][offset.y];
    else check_cells[2] = check_cells[0];
    if (IS_IN_BOUNDS(offset.x, 0, size-1) && IS_IN_BOUNDS(offset.y, 0, size-1))
        check_cells[3] = &cells[offset.x][offset.y];
    else check_cells[3] = check_cells[0];

    for (cell_index = 0; cell_index < 4; cell_index++) {
        cell    = check_cells[cell_index];
        walls   = cell->walls;
        corners = cell->corners; 
        
        for (wall_index = 0; wall_index < 4; wall_index++) {
            if (!walls[wall_index].type) continue;
            next       = (wall_index+1)%4;
            /* Push walls into the cell along their normal by the actor's radius 
            in order to create a minkowski distance */
            wall_start = Vector2Add(corners[wall_index],    Vector2Scale(Wall_Vec2_Normals[wall_index], radius));
            wall_end   = Vector2Add(corners[next], Vector2Scale(Wall_Vec2_Normals[wall_index], radius));
            
            if (CheckCollisionLines(prev_pos, new_pos, wall_start, wall_end, &collision)) {
                //DBG_OUT("Wall Start: { X: %.4f |\tY: %.4f }\tWall End: { X: %.4f |\tY: %.4f }", wall_start.x, wall_start.y, wall_end.x, wall_end.y);
                if (Vector2Distance(prev_pos,collision)<Vector2Distance(prev_pos,final_collision)) final_collision = collision;
                *collision_normal = Wall_Vec2_Normals[wall_index];
            }
        }
    }
    
    if (!IS_VECTOR2_NAN(collision)) {
        DBG_OUT("Collision: { X: %.4f\t|\tY: %.4f }", new_pos.x, new_pos.y);
        *collision_point  = final_collision;
        return true;
    }
    
    return false;
} /* Map_check_collision */
