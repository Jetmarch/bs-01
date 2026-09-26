#include "brewing_grid.h"

#include <stdlib.h>
#include <string.h>

bool BrewingGrid_Init(BrewingGrid* brewing_grid, int max_cells, int grid_width, int grid_height, int screen_width, int screen_height, int cell_size, float duration_step_s)
{
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
            cell->y = y;
            cell->x = x;
            cell->rect.x = (x * cell_size) + (((float)screen_width / 2) - ((float)grid_width * cell_size) / 2);
            cell->rect.y = (y * cell_size) + (((float)screen_height / 2) - ((float)grid_height * cell_size) / 2);
            cell->rect.width = (float)cell_size;
            cell->rect.height = (float)cell_size;
        }
    }

    Grid simple_grid = (Grid) {
        .width = grid_width,
        .height = grid_height,
        .count_of_cells = grid_width * grid_height,
        .origin = {0, 0},
        .cells = cells
    };

    Grid buffer_simple_grid = simple_grid;
    //TODO: Move malloc to grid initialize functions
    buffer_simple_grid.cells = malloc(sizeof(Cell) * max_cells);
    memcpy(buffer_simple_grid.cells, simple_grid.cells, sizeof(Cell) * simple_grid.count_of_cells);

    brewing_grid->grid = simple_grid;
    brewing_grid->buffer_grid = buffer_simple_grid;
    brewing_grid->selected_cell = NULL;
    brewing_grid->is_brewing_in_process = false;
    brewing_grid->current_duration_ms = duration_step_s;
    brewing_grid->step_duration_ms = duration_step_s;

    brewing_grid->active_grid = &brewing_grid->grid;
    brewing_grid->working_grid = &brewing_grid->buffer_grid;


    return true;
}

bool BrewingGrid_IsNeighbourExist(Grid* grid, int x, int y)
{
    Cell* cell = GetCellAtWrapAround(grid, x, y);

    return cell->type == EMPTY_CELL ? 0 : 1;
}


void BrewingGrid_Update(BrewingGrid *brewing_grid, float delta)
{
    if (brewing_grid->is_brewing_in_process) {
        brewing_grid->current_duration_ms -= delta;
        if(brewing_grid->current_duration_ms <= 0.0)
        {
            Cell* cell = NULL;
            Cell* bufferCell = NULL;
            uint8_t neighbourCount = 0;

            for (int y = 0; y < brewing_grid->grid.height; y++)
            {
                for (int x = 0; x < brewing_grid->grid.width; x++)
                {
                    //TODO: Create system for every CellType
                    neighbourCount = 0;
                    cell = GetCellAtWrapAround(brewing_grid->active_grid, x, y);
                    bufferCell = GetCellAtWrapAround(brewing_grid->working_grid, x, y);

                    neighbourCount += BrewingGrid_IsNeighbourExist(brewing_grid->active_grid, x - 1, y - 1);   // Top left
                    neighbourCount += BrewingGrid_IsNeighbourExist(brewing_grid->active_grid, x, y - 1);       // Top middle
                    neighbourCount += BrewingGrid_IsNeighbourExist(brewing_grid->active_grid, x + 1, y - 1);   // Top right
                    neighbourCount += BrewingGrid_IsNeighbourExist(brewing_grid->active_grid, x - 1, y);       // Left
                    neighbourCount += BrewingGrid_IsNeighbourExist(brewing_grid->active_grid, x + 1, y);       // Right
                    neighbourCount += BrewingGrid_IsNeighbourExist(brewing_grid->active_grid, x - 1, y + 1);   // Bottom left
                    neighbourCount += BrewingGrid_IsNeighbourExist(brewing_grid->active_grid, x, y + 1);       // Bottom middle
                    neighbourCount += BrewingGrid_IsNeighbourExist(brewing_grid->active_grid, x + 1, y + 1);   // Bottom right

                    if (neighbourCount == 3) {
                        bufferCell->type = YELLOW_CELL;
                    }
                    else if (neighbourCount == 2) {
                        bufferCell->type = cell->type;
                    }
                    else {
                        bufferCell->type = EMPTY_CELL;
                    }
                }
            }

            brewing_grid->current_duration_ms = brewing_grid->step_duration_ms;

            BrewingGrid_SwapBuffers(brewing_grid);
        }
    }
}

void BrewingGrid_SwapBuffers(BrewingGrid* brewing_grid)
{
    Grid* temp_grid = brewing_grid->active_grid;
    brewing_grid->active_grid = brewing_grid->working_grid;
    brewing_grid->working_grid = temp_grid;
}

void BrewingGrid_HandleInput(BrewingGrid* grid)
{
    Vector2 mouse_pos = GetMousePosition();

    Cell* cell = NULL;
    for (int i = 0; i < grid->grid.count_of_cells; i++) {
        cell = &grid->grid.cells[i];
        if (CheckCollisionPointRec(mouse_pos, cell->rect)) {
            cell->is_selected = true;

            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                grid->selected_cell = cell;
            }
        }
        else {
            cell->is_selected = false;
        }
    }
}

void BrewingGrid_Destroy(BrewingGrid* grid)
{
    DestroyGrid(&grid->grid);
    DestroyGrid(&grid->buffer_grid);
}
