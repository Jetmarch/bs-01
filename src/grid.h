#include <raylib.h>
#include "cell.h"


typedef struct Grid {
    int width;
    int height;
    int count_of_cells;
    int cell_size;
    Vector2 origin;
    Cell* cells;
} Grid;


Cell* GetCellAt(Grid* grid, int x, int y);
Cell* GetCellAtWrapAround(Grid* grid, int x, int y);
void DestroyGrid(Grid* grid);
void InsertGrid(Grid* target, Grid* source, int x, int y);
void Draw2DGrid(Grid* grid);
