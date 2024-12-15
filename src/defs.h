#ifndef DEFS_H
#define DEFS_H

/* Common Constant values */
#define CAMERA_HEIGHT 0.5f

/* Common Input Operations */
#define Key_Or_Button_Down( Controller, Button, Key ) (int)(IsGamepadButtonDown(Controller, Button) || IsKeyDown(Key))

/* Common Vector Manipulation */
#define Vector2_To_3( Vector, Height ) (Vector3){Vector.x, Height, Vector.y}
#define Vector3_To_2( Vector ) (Vector2){Vector.x, Vector.z}

#endif /* DEFS_H */
