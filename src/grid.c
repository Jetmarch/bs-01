#include "grid.h"
#include <stdlib.h>

Cell* GetCellAt(Grid* grid, int x, int y)
{
    return &grid->cells[y * grid->width + x];
}

//Toroidal lookup
Cell* GetCellAtWrapAround(Grid* grid, int x, int y)
{
    int t_x = (x + grid->width) % grid->width;
    int t_y = (y + grid->height) % grid->height;

    return GetCellAt(grid, t_x, t_y);
}

void InsertGrid(Grid* target, Grid* source, int x, int y)
{
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

                target->cells[ty * target->width + tx] =
                    source->cells[sy * source->width + sx];
            }
        }
}

void Draw2DGrid(Grid* grid)
{
    for(int i = 0; i < grid->count_of_cells; i++)
    {
        Cell* cell = &grid->cells[i];
        DrawRectanglePro(cell->rect, grid->origin, 0.0, SolveCellColor(cell));
    }
}

void DestroyGrid(Grid* grid)
{
    free(grid->cells);
}
