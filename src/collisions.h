#ifndef COLLISION_H
#define COLLISION_H

#define DEBUG
#include "defs.h"


/* Check collision between a circle and a line segment */
bool Collision_check_CircleSegment(Vector2 circle, float radius, Vector2 seg_start, Vector2 seg_end, Vector2 *collision, Vector2 *normal);
/* Check collision between a cast circle and line segment */
bool Collision_check_CircleCastSegment(Vector2 circ_start, Vector2 crc_end, float radius, Vector2 seg_start, Vector2 seg_end, Vector2 *collision, Vector2 *normal);

#endif /* COLLISION_H */
