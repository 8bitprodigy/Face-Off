#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

#define GET_FRUSTUM_EDGE( Position, Point )  (Vector2Add(Vector2Scale(Vector2Normalize(Vector2Subtract(Point, Position)),MAX_DRAW_DISTANCE),Position))

float Wall_Normals[4] = {
    PI,
    PI + HALF_PI,
    0,
    HALF_PI
};

bool
check_wall_frustum_intersection(Vector2 wall_start, Vector2 wall_end, Vector2 r_frust, Vector2 position, Vector2 l_frust, Vector2 *inside1, Vector2 *inside2)
{
    bool is_in_triangle = false;
    Vector2 *dummy;
    
    if (CheckCollisionPointTriangle(wall_start,r_frust,position,l_frust)) {
        inside1->x     = wall_start.x;
        inside1->y     = wall_start.y;
        is_in_triangle = true;
    }
    if (CheckCollisionPointTriangle(wall_end,  r_frust,position,l_frust)) {
        inside2->x     = wall_end.x;
        inside2->y     = wall_end.y;
        is_in_triangle = true;
    }
    if (is_in_triangle) return true;

    if (CheckCollisionLines(wall_start, wall_end, position, r_frust, dummy)) return true;
    if (CheckCollisionLines(wall_start, wall_end, position, l_frust, dummy)) return true;
    
    return false;
}

static inline bool
Index2D_equals(Index2D index1, Index2D index2)
{
    return index1.x == index2.x && index1.y == index2.y;
}

static inline bool
Buffer_contains_Index2D(Index2D *buffer, uint size, Index2D to_check)
{
    int i;
    
    if (size == 0) return false;
    printf("Current buffer size: %u\n", size);
    for (i = 0; i < size; i++) {
        if (Index2D_equals(buffer[i], to_check)) return true;
    }
    return false;
}

Cell
Cell_new()
{
    int i;
    Cell cell;
    for ( i = 0; i < 4; i++ ) {
        cell.walls[i] = (Wall){
            ORANGE,
            NONE,
            0.
        };
        cell.neighbors[i] = NULL;
        cell.corners[i] = Vector2Zero();
    }
    cell.center = Vector2Zero();
}

void 
Cell_check_vis(Cell *cell, Player *player, Vector2 r_frustum, Vector2 l_frustum, Index2D *render_buffer, uint *buffer_size)
{
    Actor   *actor      = &player->_;
    Thing   *thing      = &actor->_;

    Vector2 position = thing->position;
    Vector2 inside1;
    Vector2 inside2;
    Vector2 *tmp_1;
    Vector2 *tmp_2;

    if (Buffer_contains_Index2D(render_buffer,*buffer_size,cell->index)) return;
    render_buffer[*buffer_size] = cell->index;
    (*buffer_size)++; 

   
    tmp_1 = NULL;
    tmp_2 = NULL;
    if (cell->walls[EAST].type == NONE) {
        if (
            check_wall_frustum_intersection(
                cell->corners[0], 
                cell->corners[1], 
                r_frustum, 
                position, 
                l_frustum, 
                &tmp_1, 
                &tmp_2
            )
        ) {
            if (tmp_1) inside1 = GET_FRUSTUM_EDGE(position, *tmp_1);
            else       inside1 = r_frustum;
            if (tmp_2) inside2 = GET_FRUSTUM_EDGE(position, *tmp_2);
            else       inside2 = l_frustum;
            Cell_check_vis(cell->neighbors[EAST],player,inside1,inside2,&render_buffer,&buffer_size);
        }
    }
    tmp_1 = NULL;
    tmp_2 = NULL;
    if (cell->walls[NORTH].type == NONE) {
        if (
            check_wall_frustum_intersection(
                cell->corners[1], 
                cell->corners[2], 
                r_frustum, 
                position, 
                l_frustum, 
                &tmp_1, 
                &tmp_2
            )
        ) {
            if (tmp_1) inside1 = GET_FRUSTUM_EDGE(position, *tmp_1);
            else       inside1 = r_frustum;
            if (tmp_2) inside2 = GET_FRUSTUM_EDGE(position, *tmp_2);
            else       inside2 = l_frustum;
            Cell_check_vis(cell->neighbors[NORTH],player,inside1,inside2,&render_buffer,&buffer_size);
        }
    }
    tmp_1 = NULL;
    tmp_2 = NULL;
    if (cell->walls[WEST].type == NONE) {
        if (
            check_wall_frustum_intersection(
                cell->corners[2], 
                cell->corners[3], 
                r_frustum, 
                position, 
                l_frustum, 
                &inside1, 
                &tmp_2
            )
        ) {
            if (tmp_1) inside1 = GET_FRUSTUM_EDGE(position, *tmp_1);
            else       inside1 = r_frustum;
            if (tmp_2) inside2 = GET_FRUSTUM_EDGE(position, *tmp_2);
            else       inside2 = l_frustum;
            Cell_check_vis(cell->neighbors[WEST],player,inside1,inside2,&render_buffer,&buffer_size);
        }
    }
    tmp_1 = NULL;
    tmp_2 = NULL;
    if (cell->walls[SOUTH].type == NONE) {
        if (
            check_wall_frustum_intersection(
                cell->corners[3], 
                cell->corners[0], 
                r_frustum, 
                position, 
                l_frustum, 
                &tmp_1, 
                &tmp_2
            )
        ) {
            if (tmp_1) inside1 = GET_FRUSTUM_EDGE(position, *tmp_1);
            else       inside1 = r_frustum;
            if (tmp_2) inside2 = GET_FRUSTUM_EDGE(position, *tmp_2);
            else       inside2 = l_frustum;
            Cell_check_vis(cell->neighbors[SOUTH],player,inside1,inside2,&render_buffer,&buffer_size);
        }
    }
} /* Cell_check_vis */

void
Cell_render(Cell *cell, uint cell_width)
{
    Wall *wall_east  = &cell->walls[EAST];
    Wall *wall_north = &cell->walls[NORTH];
    Wall *wall_west  = &cell->walls[WEST];
    Wall *wall_south = &cell->walls[SOUTH];
    
    DrawPlane(
        VECTOR2_TO_3(cell->center, FLOOR_HEIGHT), 
        (Vector2){cell_width,cell_width},
        GRAY
    );
   
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
} /* Cell_render */

Map
*Map_new(const char *name, uint size, uint cell_width)
{
    Map    *map            = malloc(sizeof(Map));
    Cell   *cell;
    int    i, j, k;
    float  map_width       = size * cell_width;
    float  half_map_width  = map_width / 2.0f;
    float  half_cell_width = cell_width / 2.0f;
    float  half_size       = size / 2.0f;
    Vector2 center;

    if (!map) {
        Error_Out("Failed to allocate memory for Map\n");
        return NULL;
    }
    
    strncpy(map->name, &name, MAP_NAME_MAX_CHARS-1);
    map->name[MAP_NAME_MAX_CHARS-1] = '\0';
    map->size  = size;
    map->cells = malloc(size * sizeof(Cell *));
    if (!map->cells) {
        Error_Out("Failed to allocate memory for Map Cells.\n");
        free(map);
        return NULL;
    }
    for ( i = 0; i < size; i++ ) {
        map->cells[i] = malloc(size * sizeof(Cell));
        if (!map->cells[i]) {
            Error_Out("Failed to allocate memory for Map Cell Array.\n");
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

            for (k = 0; k < 4; k++) cell->walls[k].type = NONE;
            
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
            
            if (i < 0)    cell->neighbors[SOUTH] = &map->cells[i-1][j];
            if (i > size) cell->neighbors[NORTH] = &map->cells[i+1][j];
            if (j < 0)    cell->neighbors[WEST] = &map->cells[i][j-1];
            if (j > size) cell->neighbors[EAST] = &map->cells[i][j+1];
        }
    }
    
    return map;
} /* Map_new */

void
Map_free(Map *map)
{
    int i;
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
    int i;
    Actor   *actor      = &player->_;
    Thing   *thing      = &actor->_;
    
    Index2D render_buffer[MAX_RENDERABLE_CELLS];
    uint    buffer_size = 0;
    Index2D index       = Map_get_index(map, thing->position);
    float   r_fov_edge  = NORMALIZE(thing->rotation - player->half_fov);
    float   l_fov_edge  = NORMALIZE(thing->rotation + player->half_fov);
    Vector2 r_frustum   = Vector2Add(thing->position, Vector2Scale(ANGLE_TO_VECTOR2(r_fov_edge),MAX_DRAW_DISTANCE));
    Vector2 l_frustum   = Vector2Add(thing->position, Vector2Scale(ANGLE_TO_VECTOR2(l_fov_edge),MAX_DRAW_DISTANCE));
    

    if (index.x < 0 || map->size <= index.x || index.y < 0 || map->size <= index.y) return;
    //printf("Index : { X: %u, \tY: %u }\n",index.x,index.y);
    Cell_check_vis(&map->cells[index.x][index.y], player, r_frustum, l_frustum, &render_buffer, &buffer_size);

    for (i = 0; i < buffer_size; i++) {
        Cell_render(&map->cells[render_buffer[i].x][render_buffer[i].y], map->cell_width);
    }
    
} /* Map_render */


