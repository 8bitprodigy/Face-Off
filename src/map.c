#include <stdlib.h>
#include "map.h"

Map
*Map_new(unsigned int size)
{
} /* Map_new */

void
Map_free(Map *map)
{
    free(map->cells);
    free(map);
} /* Map_free */
