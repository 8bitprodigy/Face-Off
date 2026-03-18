#ifndef ITEM_PRIVATE_H
#define ITEM_PRIVATE_H

#include "item.h"
#include "thing_private.h"


typedef struct 
Item {
    Thing       base;

    struct Item *prev;
    struct Item *next;
    
    ItemType    type;
    ItemValue   value1;
    ItemValue   value2;
} Item;


#endif
