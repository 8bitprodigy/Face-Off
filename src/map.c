#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "map.h"

#define GET_FRUSTUM_EDGE( Position, Point )  (Vector2Add(Vector2Scale(Vector2Normalize(Vector2Subtract(Point, Position)),MAX_DRAW_DISTANCE),Position))

float Wall_Normals[4] = {
    PI,
    HALF_PI,
    0,
    PI + HALF_PI
};

Vector2 Wall_Vec2_Normals[4] = {
    (Vector2){ .x = -1.0f, .y =  0.0f },
    (Vector2){ .x =  0.0f, .y =  1.0f },
    (Vector2){ .x =  1.0f, .y =  0.0f },
    (Vector2){ .x =  0.0f, .y = -1.0f }
};

Index2D Cell_Directions[4] = {
    (Index2D){ .x =  1, .y =  0 },
    (Index2D){ .x =  0, .y = -1 },
    (Index2D){ .x = -1, .y =  0 },
    (Index2D){ .x =  0, .y =  1 },
};

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
    return !(0 <= index.x && index.x < size && 0 <= index.y && index.y < size);
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

static inline bool
Buffer_contains_Cell(Cell **buffer, uint size, Cell *cell)
{
    uint i;
    
    if (MAX_RENDERABLE_CELLS <= size) return true;
    if (size == 0) return false;
    //DBG_OUT("Current buffer size: %u", size);
    for (i = 0; i < size; i++) {
        if (buffer[i] == cell) return true;
    }
    return false;
} /* Buffer_contains_Index2D */

Cell
Cell_new()
{
    int i;
    Cell cell;
    
    for ( i = 0; i < 4; i++ ) {
        cell.walls[i] = (Wall){
            .color  = ORANGE,
            .type   = PORTAL,
            .health = 0.0f
        };
        cell.corners[i] = Vector2Zero();
    }
    cell.center = Vector2Zero();

    return cell;
} /* Cell_new */

bool
check_wall_frustum_intersection(Vector2 wall_start, Vector2 wall_end, Vector2 position, Vector2 r_frust, Vector2 l_frust, Vector2 *inside1, Vector2 *inside2)
{
    bool is_in_triangle = false;
    Vector2 dummy;
    
    //*inside1 = VECTOR2_NAN;
    //*inside2 = VECTOR2_NAN;

    //DBG_OUT("Wall Start: { X: %.4f,\tY: %.4f }", wall_start.x, wall_start.y);
    //DBG_OUT("Wall End:   { X: %.4f,\tY: %.4f }", wall_end.x,   wall_end.y);
    if (CheckCollisionPointTriangle(wall_start, position, r_frust, l_frust)) {
        *inside1       = wall_start;
        is_in_triangle = true;
    }
    if (CheckCollisionPointTriangle(wall_end, position, r_frust, l_frust)) {
        *inside2       = wall_end;
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
} /*check_wall_frustum_intersection */

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
    if (wall_east->type)  DrawTriangleStrip3D( pts_east,  4, wall_east->color);
    if (wall_north->type) DrawTriangleStrip3D( pts_north, 4, wall_north->color);
    if (wall_west->type)  DrawTriangleStrip3D( pts_west,  4, wall_west->color);
    if (wall_south->type) DrawTriangleStrip3D( pts_south, 4, wall_south->color);

#ifdef DEBUG
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

Map
*Map_new(const char *name, uint size, uint cell_width)
{
    uint    i, j, k, dir, ni, nj;
    
    Map     *map            = malloc(sizeof(Map));
    Cell    *cell;
    Wall    *walls     = cell->walls;
    
    float   map_width       = size * cell_width;
    float   half_map_width  = map_width / 2.0f;
    float   half_cell_width = cell_width / 2.0f;
    float   half_size       = size / 2.0f;
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
    for ( i = 0; i < size; i++ ) {
        map->cells[i] = malloc(size * sizeof(Cell));
        if (!map->cells[i]) {
            ERR_OUT("Failed to allocate memory for Map Cell Array.");
            Map_free(map);
            return NULL;
        }
    }
    map->cell_width = cell_width;
    
    for ( i = 0; i < size; i++ ) {
        for ( j = 0; j < size; j++ ) {
            map->cells[i][j] = Cell_new();
            cell = &map->cells[i][j];

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
                cell->walls[WEST].type   = SOLID;
                cell->walls[WEST].color  = BLUE;
            } else if (i==size-1) {
                cell->walls[EAST].type   = SOLID;
                cell->walls[EAST].color  = RED;
            }
            if (!j) {
                cell->walls[NORTH].type  = SOLID;
                cell->walls[NORTH].color = GREEN;
            } else if (j==size-1) {
                cell->walls[SOUTH].type  = SOLID;
                cell->walls[SOUTH].color = YELLOW;
            }
        }
    }
    
    return map;
} /* Map_new */

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

static inline Index2D
Map_get_index(Map *map, Vector2 position) 
{
    return (Index2D) {
        (int)((map->size * map->cell_width / 2.0f + position.x) / (float)map->cell_width),
        (int)((map->size * map->cell_width / 2.0f + position.y) / (float)map->cell_width) 
    };
}


void 
Map_check_vis(Map *map, Index2D index, Thing *thing, Vector2 r_frustum, Vector2 l_frustum, bool **render_buffer)
{
    uint    i, next;
    
    Cell    *cell      = &map->cells[index.x][index.y];
    Vector2 *corners   = cell->corners;
    Vector2 center     = cell->center;
    Wall    *walls     = cell->walls;
    bool    is_visible = false;

    Vector2 position   = thing->position;
    float   rotation   = thing->rotation;

    Vector2 inside_r;
    Vector2 inside_l;
    Index2D neighbor;

    DBG_OUT("\n\tChecking cell: { X: %u,\tY: %u }",index.x,index.y);
    //if (!cell) return;
    if (render_buffer[index.x][index.y]) return;
    render_buffer[index.x][index.y] = true;
    //DBG_OUT("Cell Index: { X: %u,\tY: %u }", cell->index.x, cell->index.y);
    //Cell_render(cell,4);

    for (i = 0; i < 4; i++) {
        neighbor = Index2D_add(index, Cell_Directions[i]);
        next     = (i+1)%4;
        
        if ( Index2D_OOB(neighbor, map->size) ) {//|| (0 < DOT(rotation, Wall_Normals[i])) ) {
            //DBG_LINE(thing->position, cell->corners[i], 0.1f, YELLOW);
            continue;
        }
        DBG_OUT("i: %u\ti+1%%4 : %u\t(i+1)%%4: %u", i, i+1%4, (i+1)%4);
        inside_r = r_frustum;
        inside_l = l_frustum;
        if (walls[i].type == PORTAL) {
            DBG_LINE(corners[i], corners[next], 0.1f, MAGENTA);
            if (
                check_wall_frustum_intersection(
                    corners[i], 
                    corners[next], 
                    position, 
                    r_frustum, 
                    l_frustum, 
                    &inside_r, 
                    &inside_l
                )
                //|| CheckCollisionPointTriangle(cell->center, position, r_frustum, l_frustum)
            ) {
                is_visible = true;
                DBG_OUT("Wall %u is visible.",i);
                //DBG_OUT("East Inside: { X: %.4f,\tY: %.4f }\n", inside_r.x, inside_r.y );
                if (!Vector2Equals(inside_r, r_frustum)) {
                    inside_r = GET_FRUSTUM_EDGE(position, inside_r);
                }
                if (!Vector2Equals(inside_l, l_frustum)) {
                    inside_l = GET_FRUSTUM_EDGE(position, inside_l);
                }
                inside_r = r_frustum; inside_l = l_frustum;
                DBG_LINE(position, inside_r,0.1f, GREEN);
                DBG_LINE(position, inside_l,0.2f, ORANGE);
                Map_check_vis(map, neighbor, thing, inside_r, inside_l, render_buffer);
            }
#ifdef DEBUG
            else {
                DBG_OUT("Wall %u is NOT visible.",i);
                DBG_LINE(position, corners[i],0.2f, RED);
            }
#endif /* DEBUG */
        }
    }
    if (is_visible) { // && !render_buffer[index.x][index.y]) {
        Cell_render(cell,4);
    } else DBG_LINE(position, center, 0.3, PURPLE);
    //render_buffer[index.x][index.y] = true;
    DBG_OUT("Cell checked.\n");
} /* Cell_check_vis */


void
Map_render(Map *map, Player *player)
{
    uint i, j;
    Actor   *actor          = player->_;
    Thing   *thing          = &actor->_;

    uint    size            = map->size;
    bool    **render_buffer = malloc(size * size * sizeof(bool));
    Index2D index           = Map_get_index(map, thing->position);
    Vector2 position        = thing->position;
    float   r_fov_edge      = NORMALIZE_ANGLE(thing->rotation + player->half_fov);
    float   l_fov_edge      = NORMALIZE_ANGLE(thing->rotation - player->half_fov);
    Vector2 r_frustum       = Vector2Add(thing->position, Vector2Scale(ANGLE_TO_VECTOR2(r_fov_edge),MAX_DRAW_DISTANCE));
    Vector2 l_frustum       = Vector2Add(thing->position, Vector2Scale(ANGLE_TO_VECTOR2(l_fov_edge),MAX_DRAW_DISTANCE));

    if (!render_buffer) {
        ERR_OUT("Could not initialize render buffer.");
        return;
    }
    for (i = 0; i < size; i++) {
        render_buffer[i] = (bool*)malloc(size*sizeof(bool));
        if (!render_buffer[i]) {
            ERR_OUT("Could not initialize render buffer inner array.");
            for(; i >= 0; i--) {
                free(render_buffer[i]);
            }
            free(render_buffer);
            return;
        }
    }
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            render_buffer[i][j] = false;
        }
    }
    //DrawTriangle3D(VECTOR2_TO_3(r_frustum,0.1f),VECTOR2_TO_3(thing->position,0.1f),VECTOR2_TO_3(l_frustum,0.1f), BLUE);
#ifdef DEBUG
    DBG_LINE(r_frustum,thing->position,0.1f,BLUE);
    DBG_LINE(thing->position,l_frustum,0.1f,BLUE);
    DBG_LINE(l_frustum,r_frustum,0.1f,BLUE);
#endif /* DEBUG */
    if (size <= index.x || size <= index.y) return;
    DBG_OUT("Index : { X: %u, \tY: %u }\n",index.x,index.y);
    Map_check_vis(map, index, thing, r_frustum, l_frustum, render_buffer);
    /*DBG_OUT("Buffer size: %u",buffer_size);
    for (i=0; i < buffer_size; i++) {
        printf("%u, ",i);
        Cell_render(&map->cells[render_buffer[i].x][render_buffer[i].y], map->cell_width);
    }*/
    DBG_OUT("frame over.\n");
    
} /* Map_render */


bool
Map_check_Actor_collision(Map *map, Actor *actor, Vector2 *collision_point)
{
    return false;
}
