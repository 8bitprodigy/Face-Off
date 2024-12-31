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

typedef struct 
Item {
    Thing    _;
    ItemType type;
    int      value;
} Item;

#endif
