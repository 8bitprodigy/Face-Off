/**********************
*                     *
*    D E F I N E S    *
*                     *
**********************/

#ifndef DEFS_H
#define DEFS_H

/* NOTES:
    CARTESIAN COORDINATE SYSTEM NOTES:
        - East  = +X
        - North = -Z
        - West  = -X
        - South = +Z
        - Rotations increase clockwise.
*/

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include <raymath.h>


/****************
    T Y P E S    
****************/
#define uint   unsigned int 
#define uint8  uint8_t
#define uint64 uint64_t


/************************
    V A R I A B L E S    
************************/
/* Common Constant values */
#define NAME_MAX_CHARS          8
#define MAP_NAME_MAX_CHARS     16
#define CAMERA_HEIGHT          (0.5f)
#define FLOOR_HEIGHT           (0.0f)
#define MAX_RENDERABLE_CELLS 1024
#define MAX_DRAW_DISTANCE    (362.0f)
#define HALF_PI             (PI/2.0f)
#define TAU                 (PI*2)

#define GAME_MAX_PLAYERS  4
#define GAME_MAX_ACTORS   32
#define GAME_MAX_ITEMS    32
#define GAME_MAX_THINGS   (GAME_MAX_PLAYERS+GAME_MAX_ACTORS+GAME_MAX_ITEMS)
#define GAME_MAX_TEXTURES 32

#define VECTOR3_UP ((Vector3){.x=0.0f,.y=1.0f,.z=0.0f})

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
#define ERR_OUT( Error_Text ) perror( "[ERROR] " Error_Text "\n" )    
#else
    #define ERR_OUT( Error_Text )
#endif

#ifdef DEBUG
    #define DBG_EXPR( expression ) expression
    #define DBG_OUT( Text, ... ) do{printf( "[DEBUG] " Text "\n", ##__VA_ARGS__ ); fflush(stdout);} while(false)
    #define DBG_LINE( vec2_1, vec2_2, height, color ) DrawLine3D(VECTOR2_TO_3( (vec2_1), (height) ), VECTOR2_TO_3( (vec2_2), (height) ), (color))
#else
    #define DBG_EXPR( expression ) 
    #define DBG_OUT( Text, ... )
    #define DBG_LINE( vec2_1, vec2_2, height, color )
#endif

/*** Common Input Operations ***/

#define GET_KEY_OR_BUTTON_PRESSED( Controller, Button, Key ) (int)(IsGamepadButtonPressed(Controller, Button) || IsKeyPressed(Key))

#define GET_KEY_OR_BUTTON_DOWN( Controller, Button, Key ) (int)(IsGamepadButtonDown(Controller, Button) || IsKeyDown(Key))

#define GET_KEY_OR_BUTTON_AXIS( Controller, Btn_Pos, Key_Pos, Btn_Neg, Key_Neg ) ( \
        GET_KEY_OR_BUTTON_DOWN( (Controller), (Btn_Pos), (Key_Pos) ) - \
        GET_KEY_OR_BUTTON_DOWN( (Controller), (Btn_Neg), (Key_Neg) )   \
    )

#define GET_KEY_OR_BUTTON_VECTOR( Controller, Btn_Pos_X, Key_Pos_X, Btn_Neg_X, Key_Neg_X, Btn_Pos_Y, Key_Pos_Y, Btn_Neg_Y, Key_Neg_Y ) \
    (Vector2){ \
        GET_KEY_OR_BUTTON_AXIS( (Controller), (Btn_Pos_X), (Key_Pos_X), (Btn_Neg_X), (Key_Neg_X) ), \
        GET_KEY_OR_BUTTON_AXIS( (Controller), (Btn_Pos_Y), (Key_Pos_Y), (Btn_Neg_Y), (Key_Neg_Y) )  \
    }

#define GET_KEY_AXIS( Key_Pos, Key_Neg )  ((int)IsKeyDown((Key_Pos)) - (int)IsKeyDown((Key_Neg)))

#define GET_KEY_VECTOR( Key_Pos_X, Key_Neg_X, Key_Pos_Y, Key_Neg_Y ) \
    (Vector2){ \
        GET_KEY_AXIS( (Key_Pos_X), (Key_Neg_X) ), \
        GET_KEY_AXIS( (Key_Pos_Y), (Key_Neg_Y) )  \
    }

#define GET_BUTTON_AXIS( Controller, Btn_Pos, Btn_Neg ) ( \
        (int)IsGamepadButtonDown((Controller), (Btn_Pos)) - \
        (int)IsGamepadButtonDown((Controller), (Btn_Neg))   \
    )

#define GET_BUTTON_VECTOR( Controller, Btn_Pos_X, Btn_Neg_X, Btn_Pos_Y, Btn_Neg_Y ) \
    (Vector2){ \
        GET_BUTTON_AXIS( (Controller, (Btn_Pos_X), (Btn_Neg_X) ), \
        GET_BUTTON_AXIS( (Controller, (Btn_Pos_Y), (Btn_Neg_Y) ), \
    }


/*** Common math functions ***/

/* Cross Product */
#define CROSS( a, b, c, d ) ((a)*(d)-(b)*(c))
/* Dot Product of two angles */
#define DOT( a, b )  \
    Vector2DotProduct( \
        (Vector2){.x=cosf((a)),.y=sinf((a))}, \
        (Vector2){.x=cosf((b)),.y=sinf((b))}  \
    )  
/* Dot Product of point to a line */
#define DOTPL( point, start, end ) ( \
    ( \
        (point.x - start.x) \
        * (end.x - start.x) \
    ) \
    + ( \
        (point.y - start.y) \
        * (end.y - start.y) \
    ) \
    / pow(Vector2Distance(start,end),2) \
)
/* Minimum value */
#define MIN( a, b ) (((a)<(b))?(a):(b))
/* Maximum value */
#define MAX( a, b ) (((b)<(a))?(a):(b))
/* Clamp value to range */
#define CLAMP( value, min, max ) ((value)<(min)?(min):((value)>(max)?(max):(value)))
/* Clamp angle to between 0 and TAU */
#define NORMALIZE_ANGLE( angle )  fmod((angle) + TAU, TAU)
/* Get sign between two numbers */
#define SIGN_BETWEEN( a, b ) (((a)<(b))-((b)<(a)))
/* Check if a number is between two others */
#define IS_IN_BOUNDS( a, b, c ) ((b)<=(a)&&(a)<=(c))


/*** Common Vector Manipulation ***/
/* Construction */
#define VECTOR2( _x_, _y_ )  ((Vector2){.x=(_x_),.y=(_y_)})
/* Convert a Vector2 to a Vector3 */
#define VECTOR2_TO_3( Vector, Height ) (Vector3){(Vector).x, (Height), (Vector).y}
/* Vector2 Cross Product */
#define VECTOR2_CROSS( Vector_1, Vector_2 ) (CROSS(Vector_1.x, Vector_1.y, Vector_2.x, Vector_2.y))
/* Convert a Vector3 to a Vector2 */
#define VECTOR3_TO_2( Vector ) (Vector2){(Vector).x, (Vector).z}
/* Convert an angle to a Vector2 */
#define ANGLE_TO_VECTOR2( Angle ) (Vector2){ cosf( (Angle) ), sinf( (Angle) ) }
/* Vector2 of NaN values */
#define VECTOR2_NAN ((Vector2){NAN,NAN})
/* Check if Vector2 is Vector2_NaN */
#define IS_VECTOR2_NAN( Vector ) (isnan((Vector).x) && isnan((Vector).y))
/* Get normal between two points */
#define VECTOR2_NORMAL( Start, End ) ( \
        Vector2Normalize( \
            Vector2Subtract( End, Start ) \
        )\
    )
/* Re-project a vector between two points to a certain length */
#define VECTOR2_REPROJECT( Start, End, Length ) \
    ( \
        Vector2Scale( \
            VECTOR2_NORMAL( End, Start ), Length \
        ) \
    )

/* Check what side of a line segment a point is on; return <0, ==0, or >0 */
#define POINT_ON_SIDE( Point, Segment_start, Segment_end ) \
    (VECTOR2_CROSS( \
        (Vector2){ \
           (Segment_end).x - (Segment_start).x, \
           (Segment_end).y - (Segment_start).y  \
        }, \
        (Vector2){ \
            (Point).x - (Segment_start).x, \
            (Point).y - (Segment_start).y  \
        } \
    ))
    
/*** Common Matrix Manipulation ***/
/* Construction */
#define MATRIX( Position, Rotation ) ( \
    MatrixMultiply( \
        MatrixTranslate( \
            (Position.x), (Position.y), (Position.z) \
        ), \
        MatrixRotateY( (Rotation) ) \
    ))


typedef struct
Vector2i
{
    union {
        int v[2];
        struct {
            int
                x,
                y;
        };
        struct {
            int
                w,
                h;
        };
    };
}
Vector2i;


typedef struct
Region
{
    union {
        Vector2i v[2];
        struct {
            Vector2i
                position,
                dimension;
        };
        struct {
            int
                x,
                y,
                w,
                h;
        };
    };
}
Region;
            

#endif /* DEFS_H */
