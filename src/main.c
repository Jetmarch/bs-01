
#include "ingredient.h"
#define RAYLIB_NUKLEAR_IMPLEMENTATION

#include <stddef.h>

#include <raylib.h>
#include <stdint.h>

#include <stdio.h>
#include "rlgl.h"
#include "external/glad.h"

#include "cauldron.h"

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
    const int cell_size = CELL_SIZE;
    const float duration_step_s = DURATION_STEP_S;

    const int font_size = 14;
    struct nk_context *ctx = InitNuklear(font_size);

    InitWindow(screen_width, screen_height, "bs-01");

    SetTargetFPS(60);

    Cauldron cauldron;

    if(!Cauldron_Init(&cauldron,grid_width, grid_height, screen_width, screen_height, cell_size, duration_step_s))
    {
        TraceLog(LOG_ERROR, "Cauldron_Init error!");
        CloseWindow();
        return 1;
    }

    IngredientList ingredients = {0};

    if(!Ingredient_InitList(&ingredients))
    {
        TraceLog(LOG_ERROR, "Ingredient_InitList error!");
        CloseWindow();
        return 1;
    }

    TraceLog(LOG_INFO, "Ingredient_InitList initialized");

    float delta;
    float frame_time;

    while(!WindowShouldClose())
    {
        frame_time = GetTime();
        UpdateNuklear(ctx);

        Cauldron_HandleInput(&cauldron);
        Cauldron_Update(&cauldron, delta);

        BeginDrawing();

        ClearBackground(BLACK);

        Cauldron_DrawIngredientsBar(&cauldron, &ingredients, ctx, screen_height);

        Grid_Draw(&cauldron.active_grid);

        if (nk_begin(ctx, "Brew", nk_rect(20, screen_height - 250, 128, 100), NK_WINDOW_BORDER)) {
            /* fixed widget pixel width */
            nk_layout_row_dynamic(ctx, 0, 1);

            //TODO:
            if(!cauldron.is_brewing_in_process) {
                if (nk_button_label(ctx, "Start brew")) {
                    cauldron.is_brewing_in_process = true;
                    cauldron.current_duration_ms = cauldron.step_duration_ms;
                }
            }
            else {
                if (nk_button_label(ctx, "End brew"))
                {
                    cauldron.is_brewing_in_process = false;
                    cauldron.current_duration_ms = cauldron.step_duration_ms;
                }
            }

            char text[64];
            snprintf(text, sizeof(text), "Next step: %.0f", cauldron.current_duration_ms);

            nk_label(ctx, text, NK_TEXT_LEFT);
        }
        nk_end(ctx);
        DrawNuklear(ctx);

        EndDrawing();


        delta = GetTime() - frame_time;
    }

    Cauldron_Destroy(&cauldron);

    // Ingredient_FreeList(&ingredients);

    UnloadNuklear(ctx);

    CloseWindow();

    return 0;
}
