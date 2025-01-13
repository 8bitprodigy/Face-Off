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

#define DEBUG
#include "defs.h"
#include "gamestate.h"
#include "map.h"


static bool done = false;

int 
main(/*int argc, char** argv*/) 
{
    Player *player;
    Map    *map;
    GameState *game_state;
    
    InitWindow (SCREEN_WIDTH, SCREEN_HEIGHT, "Face-Off!");

    game_state = GameState_new(CO_OP);
    map = Map_new("test", 16, 4);
    player = Player_new(
        (Vector2){.x=2.0f,.y=2.0f},
        0.0f, 0.5f, 0
    );
    
    GameState_add_Player(game_state, player);
    GameState_set_Map(game_state, map);

    SetTargetFPS(60);
    
    while (!done) {
        done = GET_KEY_OR_BUTTON_DOWN(0, GAMEPAD_BUTTON_MIDDLE_RIGHT, KEY_ESCAPE);
        
        GameState_update(game_state);
        
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawFPS(10,10);
            BeginMode3D(*Player_get_Camera(player));

                DrawGrid(16, 4.0f);
                
                Map_render(map, player);
                DrawSphere(VECTOR2_TO_3( Player_get_position(player), CAMERA_HEIGHT ),0.5f,YELLOW);
                
            EndMode3D();
            
        EndDrawing();
        
    }

    CloseWindow();

    Player_free(player);
    Map_free(map);
    
    return 0;
} /* main */
