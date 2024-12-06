#include "things.h"

void Thing_move(Thing *thing, Vector2 offset){
    Vector2 new_position = thing->position + offset;
    /* Todo... */
    thing->position = new_position;
}
