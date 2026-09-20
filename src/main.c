#include <stddef.h>

#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "rlgl.h"
#include "external/glad.h"

#define ENTITY_COUNT 10000

#define SCREEN_WIDTH 1240
#define SCREEN_HEIGHT 720

#define CELL_SIZE 100


// static const char *compute_shader_path = "assets/shaders/move.comp";


typedef struct
{
    float x;
    float y;
} Position;


enum CellType {
    EMPTY_CELL,
    RED_CELL,
    BLUE_CELL,
    GREEN_CELL,
    YELLOW_CELL
};

typedef struct {
    Rectangle rect;
    enum CellType type;
} Cell;

typedef struct {
    int width;
    int height;
    int count_of_cells;
    Cell* cells;
} Grid;


Cell* GetCellAt(Grid* grid, int x, int y)
{
    return &grid->cells[y + grid->width + x];
}

//Wrap around lookup
Cell* GetCellAtWrapAround(Grid* grid, int x, int y)
{
    int t_x = (x + grid->width) % grid->width;
    int t_y = (y + grid->height) % grid->height;

    return GetCellAt(grid, t_x, t_y);
}

Color SolveCellColor(Cell* cell)
{
    switch (cell->type)
    {
        case EMPTY_CELL:
            return DARKGRAY;
        case RED_CELL:
           return RED;
        case BLUE_CELL:
            return BLUE;
        case GREEN_CELL:
            return GREEN;
        case YELLOW_CELL:
            return YELLOW;
    }
}

void Draw2DGrid(Grid* grid)
{
    for(int i = 0; i < grid->count_of_cells; i++)
    {
        Cell* cell = &grid->cells[i];


        DrawRectangle(cell->rect.x, cell->rect.y, CELL_SIZE, CELL_SIZE, SolveCellColor(cell));
    }
}



int main(void)
{
    const int screen_width = SCREEN_WIDTH;
    const int screen_height = SCREEN_HEIGHT;
    const int max_cells = 9;
    const int grid_width = 3;
    const int grid_height = 3;

    InitWindow(screen_width, screen_height, "bs-01");

    SetTargetFPS(60);

    Cell* cells = malloc(sizeof(Cell) * max_cells);

    for(int i = 0; i < max_cells; i++)
    {

        if (i % 2 == 0) {
            cells[i].type = EMPTY_CELL;
        }
        else {
            cells[i].type = RED_CELL;
        }
        cells[i].rect.x = (i + grid_width) % grid_width;
        cells[i].rect.y = i;
        TraceLog(LOG_INFO, "Cell is at x: %i, y: %i, type: %i", cells[i].rect.x, cells[i].rect.y, cells[i].type);
    }

    Grid grid = {
        .width = grid_width,
        .height = grid_height,
        .count_of_cells = grid_width * grid_height,
        .cells = cells
    };

    while(!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLUE);

        Draw2DGrid(&grid);

        EndDrawing();
    }

    // UnloadShader(shader);

    CloseWindow();

    return 0;
}
