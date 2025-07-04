#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <raylib.h>
#include <string.h>
#include <float.h>
#include "collisions.h"
#include "cell_map.h"
#include "map.h"
#include "map_private.h"

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
} 
Wall;

typedef struct
Cell
{
    Wall    walls[4];
    Vector2 corners[4];
    Index2D index;
    Vector2 center;
    uint    frame_num;
    bool    is_corner[4];
} 
Cell;

typedef struct
CellMap
{
    Map  base;
    Cell **cells;
    bool **render_buffer;
    uint size;
    uint cell_width;
} 
CellMap;

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
CellMap_new(const char *name, uint size, uint cell_width)
{
    int    i, j, w;
    
    CellMap *map   = malloc(sizeof(CellMap));
    Cell    *cell;
    Wall    *walls;
    Vector2 *corners;
    bool    *is_corner;
    
    float   map_width       = size * cell_width;
    float   half_map_width  = map_width / 2.0f;
    float   half_cell_width = cell_width / 2.0f;
    //float   half_size       = size / 2.0f;
    Vector2 center;

    if (!map) {
        ERR_OUT("Failed to allocate memory for Map");
        return NULL;
    }

    Map *base = (Map*)map;
    
    strncpy(base->name, name, MAP_NAME_MAX_CHARS-1);
    base->name[MAP_NAME_MAX_CHARS-1] = '\0';
    map->size  = size;
    map->cells = malloc((size+1) * sizeof(Cell *));
    if (!map->cells) {
        ERR_OUT("Failed to allocate memory for Map Cells.");
        free(map);
        return NULL;
    }
    for ( i = 0; i < (int)size; i++ ) {
        map->cells[i] = malloc(size * sizeof(Cell));
        if (!map->cells[i]) {
            ERR_OUT("Failed to allocate memory for Map Cell Array.");
            CellMap_free(map);
            return NULL;
        }
    }
    map->cell_width = cell_width;
    
    for ( i = 0; i < (int)size; i++ ) {
        for ( j = 0; j < (int)size; j++ ) {
            Cell_init(&map->cells[i][j] );
            cell      = &map->cells[i][j];
            walls     = cell->walls;
            corners   = cell->corners;
            is_corner = cell->is_corner;

            cell->index = (Index2D){i,j};
            center = (Vector2){
                i * cell_width + half_cell_width - half_map_width,
                j * cell_width + half_cell_width - half_map_width
            };
            cell->center     = center;

            corners[0] = (Vector2) { /* SOUTHEAST */
                center.x + half_cell_width,
                center.y + half_cell_width
            };
            corners[1] = (Vector2) { /* NORTHEAST */
                center.x + half_cell_width,
                center.y - half_cell_width
            };
            corners[2] = (Vector2) { /* NORTHWEST */
                center.x - half_cell_width,
                center.y - half_cell_width
            };
            corners[3] = (Vector2) { /* SOUTHWEST */
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
                //is_corner[2] = is_corner[3] = w != PORTAL;
                
                map->cells[i-1][j].walls[EAST].type = w;
                map->cells[i-1][j].walls[EAST].color = RED;
                //map->cells[i-1][j].is_corner[2] = map->cells[i-1][j].is_corner[3] = w != PORTAL;
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

    base->free                  = &CellMap_free;
    base->render                = &CellMap_render;
    base->check_Actor_Collision = &CellMap_check_Actor_collision;
    
    return map;
} /* Map_new */


/************************************
*    M A P   D E S T R U C T O R    *
************************************/
void
CellMap_free(CellMap *cell_map)
{
    uint i;
    if (!cell_map) return;
    for ( i = 0; i < cell_map->size; i++ ) {
        free(cell_map->cells[i]);
    }
    free(cell_map->cells);
    free(cell_map);
} /* Map_free */


/******************************
*    M A P   M E T H O D S    *
******************************/
inline Index2D
CellMap_get_index(CellMap *map, Vector2 position) 
{
    return (Index2D) {
        (int)((map->size * map->cell_width / 2.0f + position.x) / (float)map->cell_width),
        (int)((map->size * map->cell_width / 2.0f + position.y) / (float)map->cell_width) 
    };
}

void
get_adjacent_cells(CellMap *map, Index2D index, Vector2 position, Cell **check_cells)
{
    int       i;
    
    int       size      = map->size-1; 
    Cell    **cells     = map->cells;
    Cell     *curr_cell = &cells[index.x][index.y];
    Cell     *swap_cell_1;
    Cell     *swap_cell_2;
    Index2D   offset;
    
    if (!( /* This is readable. I don't exactly like it, but it's readable. */
        IS_IN_BOUNDS(index.x, 0, (int)map->size-1)
        && IS_IN_BOUNDS(index.y, 0, (int)size-1)
    )) return;
    
    offset.x = SIGN_BETWEEN(position.x, curr_cell->center.x) + index.x;
    offset.y = SIGN_BETWEEN(position.y, curr_cell->center.y) + index.y;
    
    check_cells[0] = &cells[index.x][index.y];
    
    if (IS_IN_BOUNDS(offset.x, 0, size)) 
        check_cells[1] = &cells[offset.x][index.y];
    else check_cells[1] = NULL;
    
    if (IS_IN_BOUNDS(offset.y, 0, size)) 
        check_cells[2] = &cells[index.x][offset.y];
    else check_cells[2] = NULL;
    
    if (IS_IN_BOUNDS(offset.x, 0, size) && IS_IN_BOUNDS(offset.y, 0, size))
        check_cells[3] = &cells[offset.x][offset.y];
    else check_cells[3] = NULL;

    /* Sort cells to be in counter-clockwise order with southwest at index 0 */
    if (offset.x < 0) {
        swap_cell_1 = check_cells[0];
        swap_cell_2 = check_cells[2];
        
        check_cells[0] = check_cells[1];
        check_cells[2] = check_cells[3];
        
        check_cells[1] = swap_cell_1;
        check_cells[3] = swap_cell_2;
    }
    
    if (0 < offset.y) {
        swap_cell_1 = check_cells[0];
        swap_cell_2 = check_cells[1];
        
        check_cells[0] = check_cells[2];
        check_cells[1] = check_cells[3];

        check_cells[2] = swap_cell_1;
        check_cells[3] = swap_cell_2;
    }
}

bool
is_corner_on_wall(CellMap *map, Index2D index, int corner)
{
    int   i;
    Cell *cells[4];
    Cell *cell     = &map->cells[index.x][index.y]; 
    
    get_adjacent_cells(map, index, cell->corners[corner], cells);
    
    corner = 0;
    for (i = 0; i < 4; i++) {
        if (!cells[i]) return true;
        if (cells[i]->walls[corner].type || cells[i]->walls[corner-1%4].type) return true;
        corner++;
    }
    
    return false;
}

void 
check_vis(CellMap *map, Index2D index, Thing *thing, Vector2 l_frustum, Vector2 r_frustum)
{
    uint    i, next, prev;
    
    Cell    *cell           = &map->cells[index.x][index.y];
    bool    **render_buffer = map->render_buffer;
    Vector2 *corners        = cell->corners;
    //Vector2 center          = cell->center;
    Wall    *walls          = cell->walls;
    bool    is_visible      = false;

    Vector2 position   = Thing_get_position(thing);
    float   rotation   = Thing_get_rotation(thing);

    Vector2 inside_l;
    Vector2 inside_r;
    Index2D neighbor;
    
    if (render_buffer[index.x][index.y]) return;
    render_buffer[index.x][index.y] = true;

    for (i = 0; i < 4; i++) {
        neighbor = Index2D_add(index, Cell_Directions[i]);
        next     = (i+1)%4;
        prev     = (i-1)%4;
        
        
        if ( Index2D_OOB(neighbor, map->size) ) {// || (0 < DOT(rotation, Wall_Normals[i])) ) {
            continue;
        }
        
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
        ) {
            is_visible = true;
            if (walls[i].type == PORTAL) {
                
                if (!Vector2Equals(inside_l, l_frustum) && !walls[prev].type) {
                    inside_l = GET_FRUSTUM_EDGE(position, inside_l);
                } else inside_l = l_frustum;
                if (!Vector2Equals(inside_r, r_frustum) && !walls[next].type) {
                    inside_r = GET_FRUSTUM_EDGE(position, inside_r);
                } else inside_r = r_frustum;
                if (walls[prev].type == PORTAL) inside_l = l_frustum;
                if (walls[next].type == PORTAL) inside_r = r_frustum;
                //inside_r = r_frustum; inside_l = l_frustum;
                
                check_vis(map, neighbor, thing, inside_l, inside_r);
            }
        }
    }
    if (is_visible) { // && !render_buffer[index.x][index.y]) {
        Cell_render(cell,4);
    } 
} /* Cell_check_vis */


void
CellMap_render(Map *self, Player *player)
{
    CellMap *cell_map = (CellMap*)self;
    
    int    i, j;
    Thing   *thing            = THING(player);

    int    size               = cell_map->size;
    bool    **render_buffer   = cell_map->render_buffer;
    Index2D index             = CellMap_get_index(cell_map, Thing_get_position(thing));
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
    check_vis(cell_map, index, thing, l_frustum, r_frustum);
    /*DBG_OUT("Buffer size: %u",buffer_size);
    for (i=0; i < buffer_size; i++) {
        printf("%u, ",i);
        Cell_render(&map->cells[render_buffer[i].x][render_buffer[i].y], map->cell_width);
    }*/
    //DBG_OUT("frame over.\n");
    
} /* Map_render */

bool
Map_check_circle_circle_collision(
    Vector2 start_pos,
    Vector2 end_pos,
    Vector2 circle,
    float   radius,
    Vector2 *collision,
    Vector2 *normal
)
{
    if (!CheckCollisionCircleLine( circle, radius, start_pos, end_pos )) {
        return false;
    }
    
    *normal = Vector2Normalize(
        Vector2Subtract( end_pos, circle)
    );
    
    *collision = Vector2Add(Vector2Scale(*normal, radius), circle);
    end_pos = *collision;
    return true;
}

bool
Map_check_circle_segment_collision(
    Vector2 start_pos,
    Vector2 end_pos,
    float   radius,
    Vector2 seg_start,
    Vector2 seg_end,
    Vector2 *collision,
    Vector2 *normal
)
{
    return false;
}

bool
Map_check_Actor_in_broad_phase(CellMap *map, Actor *actor, Index2D index)
{
    Cell    *cell              = &map->cells[index.x][index.y];
    Vector2  cell_pos          = cell->center;
    Vector2  actor_pos         = Actor_get_position(actor);
    float    broad_phase_range = (map->cell_width/2.0f)-Actor_get_radius(actor)+0.01f;
    
    return  (  (cell_pos.x - broad_phase_range) < actor_pos.x
            &&  actor_pos.x                     < (cell_pos.x + broad_phase_range)
            && (cell_pos.y - broad_phase_range) < actor_pos.y
            &&  actor_pos.y                     < (cell_pos.y + broad_phase_range)
    );
}

bool
isCornerSolid(CellMap *map, Vector2 corner)
{
    float   half_cell_width = map->cell_width/2.0f;
    Index2D map_index = CellMap_get_index(
        map, 
        Vector2Add(
            corner, 
            VECTOR2(
                -half_cell_width, 
                half_cell_width
            )
        )
    );
    Cell *cells[4];
    
    get_adjacent_cells(map, map_index, corner, cells);
    if (cells[0] && cells[0]->walls[NORTH].type != PORTAL) return true;
    if (cells[1] && cells[1]->walls[WEST].type  != PORTAL) return true;
    if (cells[1] && cells[1]->walls[NORTH].type != PORTAL) return true;
    if (cells[2] && cells[2]->walls[WEST].type  != PORTAL) return true;
    return false;
} /* isCornerSolid */

bool
CellMap_check_Actor_collision(Map *self, Actor *actor, Vector2 new_pos, Vector2 *collision_point, Vector2 *collision_normal)
{
    CellMap *cell_map = (CellMap*)self;
    
    int   cell_index, wall_index, next;
    bool  collision_detected = false;

    int   size = cell_map->size; 
    
    Cell  **cells = cell_map->cells;
    Cell  *check_cells[4];
    Cell  *cell;
    Wall  *walls;

    Vector2 prev_pos = Actor_get_position(actor);
    float   radius   = Actor_get_radius(actor);
    Vector2 rot_prev_pos;
    Vector2 rot_new_pos;

    Vector2 collision       = VECTOR2_NAN;
    Vector2 normal          = VECTOR2_NAN;
    Vector2 final_collision = new_pos;
    Vector2 *corners;
    Vector2 wall_start;
    Vector2 wall_end;
    Vector2 mink_wall_start;
    Vector2 mink_wall_end;
    Vector2 points[4];
    
    Index2D offset;
    Index2D map_index  = CellMap_get_index(cell_map, prev_pos);
    float   rot_amount = 0;

    if (!(IS_IN_BOUNDS(map_index.x, 0, (int)size-1)&&IS_IN_BOUNDS(map_index.y, 0, (int)size-1))) return false;

    //if (!Map_check_Actor_in_broad_phase(map, actor, map_index)) return false;
    
    get_adjacent_cells(cell_map, map_index, prev_pos, check_cells);

    /* Loop through nearest four cells */
    for (cell_index = 0; cell_index < 4; cell_index++) {
        cell    = check_cells[cell_index];
        if (!cell) continue;
        walls   = cell->walls;
        corners = cell->corners; 

        /* Loop through the four walls */
        for (wall_index = 0; wall_index < 4; wall_index++) {
            next = (wall_index + 1) % 4;
            /* Check corner collision */
            bool corner_solid = isCornerSolid(cell_map, corners[wall_index]);
            //printf("Corner solid:\t%b\t| Wall #:\t%d\n", corner_solid,wall_index);
            if (
                corner_solid
                && CheckCollisionCircleLine(
                    corners[wall_index], 
                    radius, 
                    prev_pos, 
                    new_pos
                )
            ) {
                *collision_normal = Vector2Normalize(
                    Vector2Subtract(
                        new_pos, 
                        corners[wall_index]
                    )
                );
                collision = Vector2Add(Vector2Scale(*collision_normal, radius), corners[wall_index]);
                new_pos = collision;
                collision_detected = true;
            }
            /* Start Wall Collision */
            rot_amount = HALF_PI * wall_index;
            if (!walls[wall_index].type) goto handle_collision;
            /* Push walls into the cell along their normal by the actor's radius 
            in order to create a minkowski distance */
            wall_start = Vector2Rotate(
                corners[wall_index],
                rot_amount
            );
            mink_wall_start = Vector2Subtract(wall_start, VECTOR2(radius, 0));
            
            wall_end   = Vector2Rotate(
                corners[next],
                rot_amount
            );
            mink_wall_end = Vector2Subtract(wall_end, VECTOR2(radius, 0));
            
            rot_prev_pos = Vector2Rotate(prev_pos, rot_amount);
            rot_new_pos  = Vector2Rotate(new_pos,  rot_amount);

            /* If new position is outside the wall+radius boundary */
            if ( rot_new_pos.x < mink_wall_start.x) goto handle_collision;

            /* If old position is also inside the wall+radius boundary
            if ( mink_wall_start.x < rot_prev_pos.x) {
                rot_new_pos.x = mink_wall_start.x;
                collision = Vector2Rotate(rot_new_pos, -rot_amount);
                collision_detected = true;
                goto handle_collision;
            } */
            
            if ( mink_wall_start.x <= rot_new_pos.x 
                && rot_new_pos.x   <= wall_start.x
                && rot_prev_pos.y  <= wall_start.y
                && wall_end.y      <= rot_prev_pos.y 
            ) {
                *collision_normal = Wall_Vec2_Normals[wall_index];
                rot_new_pos.x = mink_wall_start.x;
                collision     = Vector2Rotate(
                    rot_new_pos,
                    -rot_amount
                );
                new_pos = collision;
                collision_detected = true;
            }
handle_collision:
            /* If there was a collision, update final_collision */
            if ( collision_detected
                && Vector2Distance(prev_pos, collision)
                <  Vector2Distance(prev_pos, final_collision)
            ) final_collision = collision;
            collision_detected = false;
        }
    }
    
    if (!IS_VECTOR2_NAN(collision)) {
        //DBG_OUT("Collision: { X: %.4f\t|\tY: %.4f }", new_pos.x, new_pos.y);
        *collision_point  = final_collision;
        *collision_normal = normal;
        Actor_on_wall(actor, *collision_point, *collision_normal);
        return true;
    }
    
    return false;
} /* Map_check_collision */
