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
#include "things.h"
#include "player.h"
#include "defs.h"


static bool done = false;

int 
main(int argc, char** argv) 
{

    InitWindow (SCREEN_WIDTH, SCREEN_HEIGHT, "Face-Off!");

    Camera camera = {0};
    camera.position   = (Vector3){ 1.0f, 0.75f, 0.0f};
    camera.target     = (Vector3){ 0.0f, 0.75f, 0.0f};
    camera.up         = (Vector3){ 0.0f, 1.0f,  0.0f};
    camera.fovy       = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    struct Player *player = Player_new(
        Vector2Zero(),
        0.0f, 0.5f, 0
    );

    float delta = 0.0f;

    SetTargetFPS(60);
    
    while (!done) {
        delta = GetFrameTime();
        done = Key_Or_Button_Down(0, GAMEPAD_BUTTON_MIDDLE_RIGHT, KEY_ESCAPE);
        //UpdateCamera( &camera, CAMERA_ORBITAL);
        Player_update(player, delta);
        camera = player->camera;
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawGrid(16, 1.0f);
                DrawCube((Vector3){  8.5f, 0.5f,  0.0f},  1.0f, 1.0f, 16.0f, RED);
                DrawCube((Vector3){  0.0f, 0.5f,  8.5f}, 16.0f, 1.0f,  1.0f, GREEN);
                DrawCube((Vector3){ -8.5f, 0.5f,  0.0f},  1.0f, 1.0f, 16.0f, BLUE);
                DrawCube((Vector3){  0.0f, 0.5f, -8.5f}, 16.0f, 1.0f,  1.0f, YELLOW);
                
            EndMode3D();
            
        EndDrawing();
        
    }

    CloseWindow();
    
    return 0;
}
