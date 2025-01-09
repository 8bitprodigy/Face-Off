#ifndef ITEM_H
#define ITEM_H

#include <raylib.h>
#include <raymath.h>


typedef enum {
    HEALTH,
    ARMOR,
    SPEED,
    COIN,
}
ItemType;

typedef union {
    int   i;
    float f;
    char  c[4];
    void  *p;
}
ItemValue;

typedef struct Item Item;

#endif
