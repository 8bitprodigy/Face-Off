/**********************
*                     *
*    D E F I N E S    *
*                     *
**********************/

#ifndef DEFS_H
#define DEFS_H


#include <stdio.h>
#include <math.h>
#include <raylib.h>


/****************
    T Y P E S    
****************/
#define uint unsigned int 

/************************
    V A R I A B L E S    
************************/
/* Common Constant values */
#define NAME_MAX_CHARS          8
#define MAP_NAME_MAX_CHARS     16
#define CAMERA_HEIGHT           0.5f
#define FLOOR_HEIGHT            0.0f
#define MAX_RENDERABLE_CELLS 1024
#define MAX_DRAW_DISTANCE     362.0f
#define HALF_PI              (PI/2.0f)
#define TAU                  (PI*2)

#ifdef PLATFORM_PSP
    #define SCREEN_WIDTH  480
    #define SCREEN_HEIGHT 272
#elifdef __DREAMCAST__
    #define SCREEN_WIDTH  640
    #define SCREEN_HEIGHT 480
#elifdef __linux__
    #define SCREEN_WIDTH  854
    #define SCREEN_HEIGHT 480
#endif

/************************
    F U N C T I O N S    
************************/

/*** Common Output Operations ***/

#ifdef __linux__
    #define ERROR_OUT( Error_Text ) perror( "[ERROR] " Error_Text "\n" )
#else
    #define ERROR_OUT( Error_Text )
#endif

#ifdef DEBUG
    #define DBG_OUT( Text, ... ) printf( "[DEBUG] " Text "\n", ##__VA_ARGS__ )
#else
    #define DBG_OUT( Text, ... )
#endif

/*** Common Input Operations ***/

#define GET_KEY_OR_BUTTON_DOWN( Controller, Button, Key ) (int)(IsGamepadButtonDown(Controller, Button) || IsKeyDown(Key))

#define GET_KEY_OR_BUTTON_AXIS( Controller, Btn_Pos, Key_Pos, Btn_Neg, Key_Neg ) ( \
        GET_KEY_OR_BUTTON_DOWN( Controller, Btn_Pos, Key_Pos ) - \
        GET_KEY_OR_BUTTON_DOWN( Controller, Btn_Neg, Key_Neg )   \
    )

#define GET_KEY_OR_BUTTON_VECTOR( Controller, Btn_Pos_X, Key_Pos_X, Btn_Neg_X, Key_Neg_X, Btn_Pos_Y, Key_Pos_Y, Btn_Neg_Y, Key_Neg_Y ) \
    (Vector2){ \
        GET_KEY_OR_BUTTON_AXIS( Controller, Btn_Pos_X, Key_Pos_X, Btn_Neg_X, Key_Neg_X ), \
        GET_KEY_OR_BUTTON_AXIS( Controller, Btn_Pos_Y, Key_Pos_Y, Btn_Neg_Y, Key_Neg_Y )  \
    }

#define GET_KEY_AXIS( Key_Pos, Key_Neg )  ((int)IsKeyDown(Key_Pos) - (int)IsKeyDown(Key_Neg))

#define GET_KEY_VECTOR( Key_Pos_X, Key_Neg_X, Key_Pos_Y, Key_Neg_Y ) \
    (Vector2){ \
        GET_KEY_AXIS( Key_Pos_X, Key_Neg_X ), \
        GET_KEY_AXIS( Key_Pos_Y, Key_Neg_Y )  \
    }

#define GET_BUTTON_AXIS( Controller, Btn_Pos, Btn_Neg ) ( \
        (int)(IsGamepadButtonDown(Controller, Btn_Pos) - \
        (int)(IsGamepadButtonDown(Controller, Btn_Neg)   \
    )

#define GET_BUTTON_VECTOR( Controller, Btn_Pos_X, Btn_Neg_X, Btn_Pos_Y, Btn_Neg_Y ) \
    (Vector2){ \
        GET_BUTTON_AXIS( Controller, Btn_Pos_X, Btn_Neg_X ), \
        GET_BUTTON_AXIS( Controller, Btn_Pos_Y, Btn_Neg_Y ), \
    }


/*** Common math functions ***/

/* Dot Product */
#define DOT( a, b )        cos(a-b)
/* Clamp angle to between 0 and TAU */
#define NORMALIZE( angle ) fmod(angle + TAU, TAU)
/* Cross Product */
#define CROSS( a, b, c, d ) (a*d-b*c)


/*** Common Vector Manipulation ***/

/* Convert a Vector2 to a Vector3 */
#define VECTOR2_TO_3( Vector, Height ) (Vector3){Vector.x, Height, Vector.y}
/* Convert a Vector3 to a Vector2 */
#define VECTOR3_TO_2( Vector ) (Vector2){Vector.x, Vector.z}
/* Convert an angle to a Vector2 */
#define ANGLE_TO_VECTOR2( Angle ) (Vector2){ cos( Angle ), sin( Angle ) }
/* Vector2 of NaN values */
#define VECTOR2_NAN ((Vector2){NAN,NAN})
/* Check if Vector2 is Vector2_NaN */
#define IS_VECTOR2_NAN( Vector ) (isnan(Vector.x) && isnan(Vector.y))

#endif /* DEFS_H */
