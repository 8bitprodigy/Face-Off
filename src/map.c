#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "map.h"

#define GET_FRUSTUM_EDGE( Position, Point )  (Vector2Add(Vector2Scale(Vector2Normalize(Vector2Subtract(Point, Position)),MAX_DRAW_DISTANCE),Position))

float Wall_Normals[4] = {
    PI,
    PI + HALF_PI,
    0,
    HALF_PI
};

static inline bool
Index2D_equals(Index2D index1, Index2D index2)
{
    return index1.x == index2.x && index1.y == index2.y;
} /* Index2D_equals */

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
        cell.neighbors[i] = INDEX2D_NAN;
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
    if (is_in_triangle) return true;

    if (CheckCollisionLines(wall_start, wall_end, position, r_frust, &dummy) ||
        CheckCollisionLines(wall_start, wall_end, position, l_frust, &dummy)) return true;
    
    return false;
} /*check_wall_frustum_intersection */

void
Cell_render(Cell *cell, uint cell_width)
{
    if (!cell) return;
    Wall *wall_east  = &cell->walls[EAST];
    Wall *wall_north = &cell->walls[NORTH];
    Wall *wall_west  = &cell->walls[WEST];
    Wall *wall_south = &cell->walls[SOUTH];
    
    DrawPlane(
        VECTOR2_TO_3(cell->center, FLOOR_HEIGHT), 
        (Vector2){cell_width,cell_width},
        GRAY
    );
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

void 
Cell_check_vis(Map *map, Index2D index, Vector2 position, Vector2 r_frustum, Vector2 l_frustum, bool **render_buffer)
{
    
    uint    i;
    
    Cell    *cell      = &map->cells[index.x][index.y];
    bool    is_visible = false;

    Vector2 inside1;
    Vector2 inside2;

    DBG_OUT("\n\tChecking cell: { X: %u,\tY: %u }",index.x,index.y);
    if (!cell) return;
    if (render_buffer[index.x][index.y]) return;
    //DBG_OUT("Cell Index: { X: %u,\tY: %u }", cell->index.x, cell->index.y);
    //Cell_render(cell,4);
    render_buffer[index.x][index.y] = true;

    for (i = 0; i < 4; i++) {
        if (Index2D_equals(cell->neighbors[i], INDEX2D_NAN)) {
            //DBG_LINE(thing->position, cell->corners[i], 0.1f, YELLOW);
            continue;
        }
        DBG_OUT("i: %u\ti+1%%4 : %u\t(i+1)%%4: %u", i, i+1%4, (i+1)%4);
        inside1 = r_frustum;
        inside2 = l_frustum;
        if (cell->walls[i].type == PORTAL) {
            if (
                check_wall_frustum_intersection(
                    cell->corners[i], 
                    cell->corners[(i+1)%4], 
                    position, 
                    r_frustum, 
                    l_frustum, 
                    &inside1, 
                    &inside2
                )
            ) {
                is_visible = true;
                DBG_OUT("Wall %u is visible.",i);
                //DBG_OUT("East Inside: { X: %.4f,\tY: %.4f }\n", inside1.x, inside1.y );
                if (Vector2Equals(inside1, r_frustum)) {
                    inside1 = GET_FRUSTUM_EDGE(position, inside1);
                }
                if (Vector2Equals(inside2, l_frustum)) {
                    inside2 = GET_FRUSTUM_EDGE(position, inside2);
                }
                //inside1 = r_frustum; inside2 = l_frustum;
                DBG_LINE(position, inside1,0.1f, GREEN);
                DBG_LINE(position, inside2,0.2f, ORANGE);
                Cell_check_vis(map, cell->neighbors[i], position, inside1, inside2, render_buffer);
            }
#ifdef DEBUG
            else {
                DBG_OUT("Wall %u is NOT visible.",i);
                DBG_LINE(position, cell->corners[i],0.2f, RED);
            }
#endif /* DEBUG */
        }
    }
    if (is_visible) {
        Cell_render(cell,4);
    }
    DBG_OUT("Cell checked.\n");
} /* Cell_check_vis */

Map
*Map_new(const char *name, uint size, uint cell_width)
{
    Map     *map            = malloc(sizeof(Map));
    Cell    *cell;
    Index2D (*neighbors)[4];
    uint    i, j, k, dir, ni, nj;
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
            neighbors = &cell->neighbors;

            cell->index = (Index2D){i,j};
            center = (Vector2){
                i * cell_width + half_cell_width - half_map_width,
                j * cell_width + half_cell_width - half_map_width
            };
            cell->center     = center;

            //for (k = 0; k < 4; k++) cell->walls[k].type = PORTAL;
            
            cell->corners[0] = (Vector2) {
                center.x + half_cell_width,
                center.y - half_cell_width
            };
            cell->corners[1] = (Vector2) {
                center.x + half_cell_width,
                center.y + half_cell_width
            };
            cell->corners[2] = (Vector2) {
                center.x - half_cell_width,
                center.y + half_cell_width
            };
            cell->corners[3] = (Vector2) {
                center.x - half_cell_width,
                center.y - half_cell_width
            };
            
            for ( dir = 0; dir < 4; dir++) {
                ni = i + (int)(dir == SOUTH) - (int)(dir == NORTH);
                nj = j + (int)(dir == WEST)  - (int)(dir == EAST);
                
                if (ni >= 0 && ni < size && nj >= 0 && nj < size) {
                    cell->neighbors[dir] = (Index2D){.x=ni,.y=nj};
                } else {
                    cell->neighbors[dir] = INDEX2D_NAN;
                }
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
Map_render(Map *map, Player *player)
{
    uint i;
    Actor   *actor          = &player->_;
    Thing   *thing          = &actor->_;

    uint    size            = map->size;
    bool    **render_buffer = malloc(size * size * sizeof(bool));
    Index2D index           = Map_get_index(map, thing->position);
    Vector2 position        = thing->position;
    float   r_fov_edge      = NORMALIZE(thing->rotation + player->half_fov);
    float   l_fov_edge      = NORMALIZE(thing->rotation - player->half_fov);
    Vector2 r_frustum       = Vector2Add(thing->position, Vector2Scale(ANGLE_TO_VECTOR2(r_fov_edge),MAX_DRAW_DISTANCE));
    Vector2 l_frustum       = Vector2Add(thing->position, Vector2Scale(ANGLE_TO_VECTOR2(l_fov_edge),MAX_DRAW_DISTANCE));

    if (!render_buffer) {
        ERR_OUT("Could not initialize render buffer.");
        return;
    }
    for (i = 0; i < map->size; i++) {
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
    //DrawTriangle3D(VECTOR2_TO_3(r_frustum,0.1f),VECTOR2_TO_3(thing->position,0.1f),VECTOR2_TO_3(l_frustum,0.1f), BLUE);
#ifdef DEBUG
    DBG_LINE(r_frustum,thing->position,0.1f,BLUE);
    DBG_LINE(thing->position,l_frustum,0.1f,BLUE);
    DBG_LINE(l_frustum,r_frustum,0.1f,BLUE);
#endif /* DEBUG */
    if (map->size <= index.x || map->size <= index.y) return;
    //DBG_OUT("Index : { X: %u, \tY: %u }\n",index.x,index.y);
    Cell_check_vis(map, index, position, r_frustum, l_frustum, render_buffer);
    /*DBG_OUT("Buffer size: %u",buffer_size);
    for (i=0; i < buffer_size; i++) {
        printf("%u, ",i);
        Cell_render(&map->cells[render_buffer[i].x][render_buffer[i].y], map->cell_width);
    }*/
    DBG_OUT("frame over.\n");
    
} /* Map_render */


