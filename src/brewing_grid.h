#include "cell.h"


typedef struct BrewingGrid {
    Grid grid;
    Grid buffer_grid;
    bool is_brewing_in_process;
    float step_duration_ms;
    float current_duration_ms;

    Cell* selected_cell;

    Grid* active_grid;
    Grid* working_grid;
} BrewingGrid;


bool BrewingGrid_Init(BrewingGrid* brewing_grid, int max_cells, int grid_width, int grid_height, int screen_width, int screen_height, int cell_size, float duration_step_s);
bool BrewingGrid_IsNeighbourExist(Grid* grid, int x, int y);
void BrewingGrid_Update(BrewingGrid* grid, float delta);
void BrewingGrid_HandleInput(BrewingGrid* grid);
void BrewingGrid_SwapBuffers(BrewingGrid* grid);
void BrewingGrid_Destroy(BrewingGrid* grid);
