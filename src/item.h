#ifndef ITEM_H
#define ITEM_H

#include <raylib.h>
#include <raymath.h>


typedef enum {
    HEALTH,
    ARMOR,
    SPEED,
}
ItemType;

typedef union {
    int   i;
    float f;
    void  *p;
}
ItemValue;

typedef struct Item Item;

#endif
