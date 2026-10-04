#ifndef GRID_H
#define GRID_H

#include <raylib.h>
#include "cell.h"

typedef struct Grid
{
    int width;
    int height;
    int count_of_cells;
    int cell_size;
    int count_of_active_materials;
    Vector2 origin;
    Cell *cells;
} Grid;

bool Grid_Init(Grid *grid, int width, int height, Vector2 origin, int cell_size);
Cell *Grid_GetCellAt(Grid *grid, int x, int y);
Cell *Grid_GetCellAtWrapAround(Grid *grid, int x, int y);
void Grid_Draw(Grid *grid, float delta);
bool Grid_InsertGrid(Grid *target, Grid *source, int x, int y);
void Grid_Destroy(Grid *grid);

#endif
