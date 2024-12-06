#ifndef FACE_OFF_H
#define FACE_OFF_H

#include "things.h"
#include "player.h"


#ifdef PLATFORM_PSP
    #define SCREEN_WIDTH  480
    #define SCREEN_HEIGHT 272
#else
    #define SCREEN_WIDTH  640
    #define SCREEN_HEIGHT 480
#endif

typedef struct {
} Gamestate;

#endif /* FACE_OFF_H */
