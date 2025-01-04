#ifndef ITEM_H
#define ITEM_H

#include <raylib.h>
#include <raymath.h>
#include "thing.h"
//#define DEBUG
#include "defs.h"


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

typedef struct 
Item {
    Thing       _;

    struct Item *prev;
    struct Item *next;
    
    ItemType    type;
    ItemValue   value1;
    ItemValue   value2;
} Item;

#endif
