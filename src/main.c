#define RAYLIB_NUKLEAR_IMPLEMENTATION

#include <stddef.h>

#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>
#include "rlgl.h"
#include "external/glad.h"
#include "cell.h"

#include <raylib-nuklear/include/raylib-nuklear.h>


#define ENTITY_COUNT 10000

#define SCREEN_WIDTH 1240
#define SCREEN_HEIGHT 720

#define CELL_SIZE 100



int main(void)
{
    const int screen_width = SCREEN_WIDTH;
    const int screen_height = SCREEN_HEIGHT;
    const int max_cells = 9;
    const int grid_width = 3;
    const int grid_height = 3;

    const int font_size = 14;
    struct nk_context *ctx = InitNuklear(font_size);

    InitWindow(screen_width, screen_height, "bs-01");

    SetTargetFPS(60);

    Cell* cells = malloc(sizeof(Cell) * max_cells);

    int n_i = 0;
    Cell* cell = NULL;
    for (int y = 0; y < grid_height; y++)
    {
        for(int x = 0; x < grid_width; x++)
        {
            n_i =  y + grid_width * x;
            cell = &cells[n_i];

            if (n_i % 2 == 0) {
                cell->type = EMPTY_CELL;
            }
            else {
                cell->type = RED_CELL;
            }

            cell->rect.x = x * CELL_SIZE;
            cell->rect.y = y * CELL_SIZE;
            cell->rect.width = (float)CELL_SIZE;
            cell->rect.height = (float)CELL_SIZE;

            // TraceLog(LOG_INFO, "Cell x: %.0f y: %.0f, w: %.0f, h: %.0f type: %i", cell->rect.x, cell->rect.y, cell->rect.width, cell->rect.height, cell->type);
        }
    }

    Grid grid = {
        .width = grid_width,
        .height = grid_height,
        .count_of_cells = grid_width * grid_height,
        .origin = {0, 0},
        .cells = cells
    };

    Cell* currentCell = NULL;

    while(!WindowShouldClose())
    {
        UpdateNuklear(ctx);

        Vector2 mouse_pos = GetMousePosition();

        Cell* cell = NULL;
        for (int i = 0; i < grid.count_of_cells; i++)
        {
            cell = &grid.cells[i];
            if (CheckCollisionPointRec(mouse_pos, cell->rect))
            {
                cell->is_selected = true;

                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    TraceLog(LOG_INFO, "Cell %i was clicked", cell->type);
                    currentCell = cell;
                }
            }
            else {
                cell->is_selected = false;
            }
        }
        // There will be logic for updating the grid by the rules of the game of life



        BeginDrawing();

        ClearBackground(BLUE);

        Draw2DGrid(&grid);

        if(currentCell != NULL)
        {
            DrawCellInfo(currentCell, screen_width, screen_height, ctx);
            DrawCellButtons(ctx, screen_height, currentCell);
        }

        DrawNuklear(ctx);

        EndDrawing();
    }

    free(cells);

    UnloadNuklear(ctx);

    CloseWindow();

    return 0;
}
