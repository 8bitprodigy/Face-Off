#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"


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

Map
*Map_new(const char *name, uint size, uint cell_width)
{
    Map  *map   = malloc(sizeof(Map));
    int i, j;
    float half_cell_width = (float)cell_width/2.0f;
    float half_size       = (float)size/2.0f;

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

    for ( i = 0; i < size; i++ ) {
        for ( j = 0; j < size; j++ ) {
            map->cells[i][j] = Cell_new();
            map->cells[i][j].center = (Vector2){
                i * cell_width + half_cell_width - half_size,
                j * cell_width + half_cell_width - half_size
            };
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

void 
Cell_render(Cell *cell, uint cell_width)
{
    DrawPlane(
        Vector2_To_3(cell->center, FLOOR_HEIGHT), 
        (Vector2){cell_width,cell_width},
        GRAY
    );
} /* Cell_render */

void
Map_render(Map *map, Player *player)
{
    Actor *actor = &player->_;
    Thing *thing = &actor->_;
    Index2D index = Map_Get_Index(map, thing->position);

    printf("Index | X: %d \tY: %d\n",index.x,index.y);
    Cell_render(&map->cells[index.x][index.y], map->cell_width);
    
} /* Map_render */


