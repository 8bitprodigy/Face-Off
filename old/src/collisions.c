#include "collisions.h"


bool
Collision_check_CircleSegment(
    Vector2 circle, 
    float   radius, 
    Vector2 start, 
    Vector2 end, 
    Vector2 *intersection, 
    Vector2 *normal
) 
{
    Vector2 diff = Vector2Subtract(end, start);
    Vector2 from = Vector2Subtract(start, circle);

    float a = diff.x * diff.x + diff.y * diff.y;
    float b = 2 * (from.x * diff.x + from.y * diff.y);
    float c = from.x * from.x + from.y * from.y - radius * radius;

    /* Solve the quadratic equation ax^2 + bx + c = 0 */
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        DBG_OUT("No intersection found");
        // No intersection
        return false;
    }

    discriminant = sqrt(discriminant);

    /* Calculate the smaller t value (first intersection) */
    float t = (-b - discriminant) / (2 * a);

    /* Since the start point is outside the circle and the end point is inside, */
    /* t will always be in the range [0, 1]. */
    intersection->x = start.x + t * diff.x;
    intersection->y = start.y + t * diff.y;

    *normal = VECTOR2_NORMAL(circle, *intersection);
    DBG_OUT("Intersection found.");
    return true;
}

bool
Collision_get_CircleSegmentIntersection( 
    Vector2 circle, 
    float   radius, 
    Vector2 seg_start, 
    Vector2 seg_end, 
    Vector2 *collision, 
    Vector2 *normal
)
{
    float   dot     = DOTPL(circle, seg_start, seg_end);
    Vector2 closest =  VECTOR2(
        seg_start.x + (dot * (seg_end.x - seg_start.x)),
        seg_start.y + (dot * (seg_end.y - seg_start.y))
    );

    /* Check if closest point is outside line */
    if (!CheckCollisionPointLine(closest, seg_start, seg_end, 0)) {
        DBG_OUT("Closest point is outside line segment.");
        return false;
    }
    
    /* Check if closest point falls outside of radius */ 
    if (radius < Vector2Distance(circle, closest)) {
        DBG_OUT("Closest point is outside endpoint radius");
        return false;
    }
    
    /* Check if start point falls inside circle */ 
    if (Vector2Distance(circle, seg_start) < radius) {
        DBG_OUT("Start point falls outside circle.");
        *normal    = Vector2Normalize(Vector2Subtract(seg_start, circle));
        *collision = Vector2Add(Vector2Scale(*normal, radius),circle);
        return true;
    }

    return Collision_check_CircleSegment(
        circle, 
        radius, 
        seg_start, 
        seg_end, 
        collision, 
        normal
    );
} 

bool
Collision_check_CircleCastSegment(
    Vector2 circ_start, 
    Vector2 circ_end, 
    float   radius, 
    Vector2 seg_start, 
    Vector2 seg_end, 
    Vector2 *collision, 
    Vector2 *normal
)
{
    Vector2 local_norm;
    float   dot_end     = DOTPL(circ_end, seg_start, seg_end);
    Vector2 closest_end = VECTOR2(
        seg_start.x + (dot_end * (seg_end.x - seg_start.x)),
        seg_start.y + (dot_end * (seg_end.y - seg_start.y))
    );
    float   dot_start     = DOTPL(circ_start, seg_start, seg_end);
    Vector2 closest_start = VECTOR2(
        seg_start.x + (dot_start * (seg_end.x - seg_start.x)),
        seg_start.y + (dot_start * (seg_end.y - seg_start.y))
    );

    /* Check collision against segment ends */
    if (!CheckCollisionPointLine(closest_end, seg_start, seg_end, 0)) {
        DBG_OUT("Outside segment ends.");
        closest_end = (
            Vector2Distance(closest_end, seg_start)
            < Vector2Distance(closest_end, seg_end)
        ) ? seg_start : seg_end; 

        return Collision_check_CircleSegment(
            closest_end, 
            radius, 
            seg_start, 
            seg_end, 
            collision, 
            normal
        );
    }

    /* Check collision against segment belly */
    if (radius < Vector2Distance(closest_end, circ_end)) {
        DBG_OUT("End point outside segment radius");
        return false;
    }

    local_norm = VECTOR2_NORMAL(closest_end, circ_end);
    *normal    = *normal;

    /* Check if start point is also inside radius */
    if (radius < Vector2Distance(closest_start, circ_start)) {
        DBG_OUT("Start point outside segment radius.");
        /* We're just gonna push the end position out to the edge, fuggit */
        *collision = VECTOR2_REPROJECT(closest_end, circ_end, radius);
        return true;
    }

    DBG_OUT("Collision with segment belly");
    /* Might as well precalculate the offset we're going to adjust the segment by */
    local_norm = Vector2Scale(local_norm, radius);
    
    return CheckCollisionLines(
        circ_start, 
        circ_end, 
        Vector2Add( seg_start, local_norm),
        Vector2Add( seg_end,   local_norm),
        collision
    );
}
