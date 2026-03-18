#ifndef THING_H
#define THING_H

#include <raylib.h>
#include <raymath.h>


#define THING( self ) ((Thing*)(self))

#define BODY_NONE()           ((Body){.type = NONE})
#define BODY_MESH(   Mesh   ) ((Body){.type = MESH,   .data.mesh   = (Mesh)})
#define BODY_SPRITE( Sprite ) ((Body){.type = SPRITE, .data.sprite = (Sprite)})

typedef enum 
{
    NONE,
    MESH,
    SPRITE,
}
BodyType;

typedef struct
Body
 {
    BodyType type;
    union 
    {
        Mesh      *mesh;
        Texture2D *sprite;
    }
    data;
}
Body;

typedef struct Thing Thing;

/* Constructors */
void  Thing_init(Thing *thing, Body    body,     Vector2 position, float rotation, float radius);
Thing *Thing_new(Body   body,  Vector2 position, float   rotation, float radius);
/* Destructor */
void Thing_free(Thing *thing);

/* Getters */
Thing   *Thing_get_prev(   Thing *thing);
Thing   *Thing_get_next(   Thing *thing);
Vector2 Thing_get_position(Thing *thing);
float   Thing_get_rotation(Thing *thing);

/* List Operations */
void Thing_insert(Thing *thing1, Thing *thing2); /* pun intended */
void Thing_remove(Thing *thing);

/* Thing Methods */
void Thing_draw(Thing *thing, Camera *camera);

#endif /* THING_H */
