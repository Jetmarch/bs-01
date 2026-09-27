#include "cell.h"

typedef struct Cauldron {
    Grid grid;
    Grid buffer_grid;
    bool is_brewing_in_process;
    float step_duration_ms;
    float current_duration_ms;

    Cell* selected_cell;

    Grid* active_grid;
    Grid* working_grid;


    Grid* selected_ingredient;
} Cauldron;


bool Cauldron_Init(Cauldron* brewing_grid, int max_cells, int grid_width, int grid_height, int screen_width, int screen_height, int cell_size, float duration_step_s);
bool Cauldron_IsNeighbourExist(Grid* grid, int x, int y);
void Cauldron_Update(Cauldron* grid, float delta);
void Cauldron_HandleInput(Cauldron* grid);
void Cauldron_Destroy(Cauldron* grid);
void DrawCellInfo(const Cell* cell, const int screen_width, const int screen_height, struct nk_context* ctx);
