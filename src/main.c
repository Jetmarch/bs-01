
#define RAYLIB_NUKLEAR_IMPLEMENTATION

#include <stddef.h>

#include <raylib.h>
#include <stdint.h>

#include <stdio.h>
#include "rlgl.h"
#include "external/glad.h"

#include "brewing_grid.h"

#include <raylib-nuklear/include/raylib-nuklear.h>


#define ENTITY_COUNT 10000

#define SCREEN_WIDTH 1240
#define SCREEN_HEIGHT 720

#define CELL_SIZE 30

#define DURATION_STEP_S 0.3


int main(void)
{
    const int screen_width = SCREEN_WIDTH;
    const int screen_height = SCREEN_HEIGHT;
    const int grid_width = 15;
    const int grid_height = 15;
    const int max_cells = grid_width * grid_height;

    const int font_size = 14;
    struct nk_context *ctx = InitNuklear(font_size);

    InitWindow(screen_width, screen_height, "bs-01");

    SetTargetFPS(60);

    BrewingGrid brewing_grid;

    if(!BrewingGrid_Init(&brewing_grid, max_cells, grid_width, grid_height, screen_width, screen_height, CELL_SIZE, DURATION_STEP_S))
    {
        TraceLog(LOG_ERROR, "BrewingGrid init error!");
        CloseWindow();
        return 1;
    }

    float delta;
    float frame_time;

    while(!WindowShouldClose())
    {
        frame_time = GetTime();
        UpdateNuklear(ctx);


        BrewingGrid_HandleInput(&brewing_grid);
        BrewingGrid_Update(&brewing_grid, delta);



        BeginDrawing();

        ClearBackground(BLACK);

        Draw2DGrid(&brewing_grid.grid);

        if(brewing_grid.selected_cell != NULL)
        {
            DrawCellInfo(brewing_grid.selected_cell, screen_width, screen_height, ctx);
            DrawCellButtons(ctx, screen_height, brewing_grid.selected_cell);
        }

        if (nk_begin(ctx, "Brew", nk_rect(20, screen_height - 250, 128, 100), NK_WINDOW_BORDER)) {
            /* fixed widget pixel width */
            nk_layout_row_dynamic(ctx, 0, 1);

            if(!brewing_grid.is_brewing_in_process) {
                if (nk_button_label(ctx, "Start brew")) {
                    brewing_grid.is_brewing_in_process = true;
                    brewing_grid.current_duration_ms = brewing_grid.step_duration_ms;
                }
            }
            else {
                if (nk_button_label(ctx, "End brew"))
                {
                    brewing_grid.is_brewing_in_process = false;
                    brewing_grid.current_duration_ms = brewing_grid.step_duration_ms;
                }
            }

            char text[64];
            snprintf(text, sizeof(text), "Next step: %.0f", brewing_grid.current_duration_ms);

            nk_label(ctx, text, NK_TEXT_LEFT);
        }
        nk_end(ctx);
        DrawNuklear(ctx);

        EndDrawing();


        delta = GetTime() - frame_time;
    }

    BrewingGrid_Destroy(&brewing_grid);

    UnloadNuklear(ctx);

    CloseWindow();

    return 0;
}
