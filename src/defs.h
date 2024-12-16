#ifndef DEFS_H
#define DEFS_H

/* Common Constant values */
#define CAMERA_HEIGHT 0.5f

/* Common Input Operations */
#define Get_Key_Or_Button_Down( Controller, Button, Key ) (int)(IsGamepadButtonDown(Controller, Button) || IsKeyDown(Key))
#define Get_Key_Or_Button_Axis( Controller, Btn_Pos, Key_Pos, Btn_Neg, Key_Neg) (\
        Get_Key_Or_Button_Down( Controller, Btn_Pos, Key_Pos ) - \
        Get_Key_Or_Button_Down( Controller, Btn_Neg, Key_Neg )   \
    )
#define Get_Key_Or_Button_Vector( Controller, Btn_Pos_X, Key_Pos_X, Btn_Neg_X, Key_Neg_X, Btn_Pos_Y, Key_Pos_Y, Btn_Neg_Y, Key_Neg_Y) \
    (Vector2){ \
        Get_Key_Or_Button_Axis( Controller, Btn_Pos_X, Key_Pos_X, Btn_Neg_X, Key_Neg_X ), \
        Get_Key_Or_Button_Axis( Controller, Btn_Pos_Y, Key_Pos_Y, Btn_Neg_Y, Key_Neg_Y )  \
    }
#define Get_Key_Axis( Key_Pos, Key_Neg )  ((int)IsKeyDown(Key_Pos) - (int)IsKeyDown(Key_Neg))
#define Get_Key_Vector( Key_Pos_X, Key_Neg_X, Key_Pos_Y, Key_Neg_Y ) \
    (Vector2){ \
        Get_Key_Axis( Key_Pos_X, Key_Neg_X ), \
        Get_Key_Axis( Key_Pos_Y, Key_Neg_Y )  \
    }
#define Get_Button_Axis( Controller, Btn_Pos, Btn_Neg ) (\
         (int)(IsGamepadButtonDown(Controller, Btn_Pos) - \
         (int)(IsGamepadButtonDown(Controller, Btn_Neg)   \
    )
#define Get_Button_Vector( Controller, Btn_Pos_X, Btn_Neg_X, Btn_Pos_Y, Btn_Neg_Y) \
    (Vector2){ \
        Get_Button_Axis( Controller, Btn_Pos_X, Btn_Neg_X ), \
        Get_Button_Axis( Controller, Btn_Pos_Y, Btn_Neg_Y ), \
    }

/* Common Vector Manipulation */
#define Vector2_To_3( Vector, Height ) (Vector3){Vector.x, Height, Vector.y}
#define Vector3_To_2( Vector ) (Vector2){Vector.x, Vector.z}

#endif /* DEFS_H */
