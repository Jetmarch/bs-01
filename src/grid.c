#include "grid.h"
#include "cell.h"
#include "raylib.h"
#include <assert.h>
#include <stdlib.h>

bool Grid_Init(Grid* grid, int width, int height, Vector2 origin, int cell_size)
{
    assert(grid != NULL);
    grid->width = width;
    grid->height = height;
    grid->count_of_cells = width * height;
    grid->origin = origin;

    grid->cell_size = cell_size;

    grid->cells = malloc(sizeof(Cell) * width * height);

    if (grid->cells == NULL)
    {
        return false;
    }

    return true;
}

Cell* Grid_GetCellAt(Grid* grid, int x, int y)
{
    assert(grid != NULL);
    if(x < 0 || x >= grid->width ||
        y < 0 || y >= grid->height)
    {
        return NULL;
    }

    return &grid->cells[y * grid->width + x];
}

//Toroidal lookup
Cell* Grid_GetCellAtWrapAround(Grid* grid, int x, int y)
{
    assert(grid != NULL);
    int t_x = (x + grid->width) % grid->width;
    int t_y = (y + grid->height) % grid->height;

    return Grid_GetCellAt(grid, t_x, t_y);
}

bool Grid_InsertGrid(Grid* target, Grid* source, int x, int y)
{
    assert(target != NULL);
    assert(source != NULL);

    if(x < 0 || x >= target->width ||
        y < 0 || y >= target->height)
    {
        return false;
    }

    for (int sy = 0; sy < source->height; sy++)
    {
        for (int sx = 0; sx < source->width; sx++)
        {
            int tx = x + sx;
            int ty = y + sy;

            if (tx < 0 || tx >= target->width ||
                ty < 0 || ty >= target->height)
            {
                continue;
            }
            //Copy only type
            Cell_CopyContent(&source->cells[sy * source->width + sx], &target->cells[ty * target->width + tx]);
        }
    }

    return true;
}

void Grid_Draw(Grid* grid)
{
    assert(grid != NULL);
    for(int i = 0; i < grid->count_of_cells; i++)
    {
        Cell* cell = &grid->cells[i];
        assert(cell != NULL);
        DrawRectanglePro(cell->rect, grid->origin, 0.0, SolveCellColor(cell));
    }
}

void Grid_Destroy(Grid* grid)
{
    assert(grid != NULL);
    free(grid->cells);
    grid->cells = NULL;
}
