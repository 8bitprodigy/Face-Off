/*******************************************************************************
*                                                                              *
*                            F A C E - O F F                                   *
*                                                                              *
********************************************************************************
*                                                                              *
* This is unencumbered software, released into the public domain.              *
* For jurisdictions where the public domain is not applicable, it is released  *
* under the following license:                                                 *
*                                                                              *
* Zero-Clause BSD                                                              *
* ===============                                                              *
*                                                                              *
* Permission to use, copy, modify, and/or distribute this software for         *
* any purpose with or without fee is hereby granted.                           *
*                                                                              *
* THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL                *
* WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES     *
* OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE       *
* FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY           *
* DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   *
* AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT      *
* OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.            *
*                                                                              *
*******************************************************************************/
#include <raylib.h>
#include "face-off.h"


static bool done = false;

int main(int argc, char** argv) {

    InitWindow (SCREEN_WIDTH, SCREEN_HEIGHT, "Face-Off!");

    Camera camera = {0};
    camera.position   = (Vector3){ 1.0f, 0.75f, 0.0f};
    camera.target     = (Vector3){ 0.0f, 0.75f, 0.0f};
    camera.up         = (Vector3){ 0.0f, 1.0f,  0.0f};
    camera.fovy       = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);
    
    while (!done) {

        done = IsKeyPressed(KEY_ESCAPE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT);
        UpdateCamera( &camera, CAMERA_ORBITAL);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawGrid(10, 1.0f);
                DrawCube((Vector3){ 10.0f, 0.5f,   0.0f},1.0f,1.0f,1.0f, RED);
                DrawCube((Vector3){  0.0f, 0.5f,  10.0f},1.0f,1.0f,1.0f, GREEN);
                DrawCube((Vector3){-10.0f, 0.5f,   0.0f},1.0f,1.0f,1.0f, BLUE);
                DrawCube((Vector3){  0.0f, 0.5f, -10.0f},1.0f,1.0f,1.0f, YELLOW);
                
            EndMode3D();
            
        EndDrawing();
        
    }

    CloseWindow();
    
    return 0;
}
