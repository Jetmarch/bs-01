#include "cauldron.h"
#include "cell.h"
#include "grid.h"
#include "ingredient.h"
#include "raylib.h"

#include <string.h>
#include <stdio.h>

bool Cauldron_Init(Cauldron* cauldron, int grid_width, int grid_height, int screen_width, int screen_height, int cell_size, float duration_step_s)
{
    Vector2 grid_origin = {
        -(((float)screen_width / 2) - ((float)grid_width * cell_size) / 2),
        -(((float)screen_height / 2) - ((float)grid_height * cell_size) / 2)
    };

    Grid active_grid = {0};
    Grid_Init(&active_grid, grid_width, grid_height, grid_origin, cell_size);

    int n_i = 0;
    Cell* cell = NULL;
    for (int y = 0; y < grid_height; y++)
    {
        for(int x = 0; x < grid_width; x++)
        {
            n_i =  y + grid_width * x;
            cell = &active_grid.cells[n_i];

            if (n_i % 2 == 0) {
                cell->type = EMPTY_CELL;
            }
            else {
                cell->type = RED_CELL;
            }
            cell->y = y;
            cell->x = x;
            cell->rect.x = (x * cell_size);// + (((float)screen_width / 2) - ((float)grid_width * cell_size) / 2);
            cell->rect.y = (y * cell_size);// + (((float)screen_height / 2) - ((float)grid_height * cell_size) / 2);
            cell->rect.width = (float)cell_size;
            cell->rect.height = (float)cell_size;

            PrintCell(cell);
        }
    }

    TraceLog(LOG_INFO, "Origin x:%.0f, y:%.0f", grid_origin.x, grid_origin.y);

    Grid buffer_grid;
    Grid_Init(&buffer_grid, grid_width, grid_height, grid_origin, cell_size);
    memcpy(buffer_grid.cells, active_grid.cells, sizeof(Cell) * active_grid.count_of_cells);

    cauldron->active_grid = active_grid;
    cauldron->buffer_grid = buffer_grid;
    cauldron->selected_cell = NULL;
    cauldron->selected_ingredient = NULL;
    cauldron->is_brewing_in_process = false;
    cauldron->current_duration_ms = duration_step_s;
    cauldron->step_duration_ms = duration_step_s;

    cauldron->active_grid_ptr = &cauldron->active_grid;
    cauldron->buffer_grid_ptr = &cauldron->buffer_grid;


    return true;
}

bool Cauldron_IsNeighbourExist(Grid* grid, int x, int y)
{
    Cell* cell = Grid_GetCellAtWrapAround(grid, x, y);

    return cell->type == EMPTY_CELL ? 0 : 1;
}

int Cauldron_GetNeighbourCount(Cauldron* brewing_grid, int x, int y)
{
    uint8_t neighbourCount = 0;

    neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x - 1, y - 1);   // Top left
    neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x, y - 1);       // Top middle
    neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x + 1, y - 1);   // Top right
    neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x - 1, y);       // Left
    neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x + 1, y);       // Right
    neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x - 1, y + 1);   // Bottom left
    neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x, y + 1);       // Bottom middle
    neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x + 1, y + 1);

    return neighbourCount;
}

void Cauldron_SwapBuffers(Cauldron* brewing_grid)
{
    Grid* temp_grid = brewing_grid->active_grid_ptr;
    brewing_grid->active_grid_ptr = brewing_grid->buffer_grid_ptr;
    brewing_grid->buffer_grid_ptr = temp_grid;
}


void Cauldron_Update(Cauldron *brewing_grid, float delta)
{
    if (brewing_grid->is_brewing_in_process) {
        brewing_grid->current_duration_ms -= delta;
        if(brewing_grid->current_duration_ms <= 0.0)
        {

            uint8_t neighbourCount = 0;
            Cell* bufferCell = NULL;
            Cell* cell;
            for (int y = 0; y < brewing_grid->active_grid.height; y++)
            {
                for (int x = 0; x < brewing_grid->active_grid.width; x++)
                {
                    //TODO: Create system for every CellType
                    neighbourCount = 0;
                    cell = Grid_GetCellAtWrapAround(brewing_grid->active_grid_ptr, x, y);
                    bufferCell = Grid_GetCellAtWrapAround(brewing_grid->buffer_grid_ptr, x, y);

                    neighbourCount = Cauldron_GetNeighbourCount(brewing_grid, x, y);

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

            Cauldron_SwapBuffers(brewing_grid);
        }
    }
}

bool Cauldron_InsertIngredient(Cauldron* cauldron, Ingredient* ingredient, int x, int y)
{
    Grid_InsertGrid(&cauldron->buffer_grid, &ingredient->grid, x, y);
    return Grid_InsertGrid(&cauldron->active_grid, &ingredient->grid, x, y);
}


void Cauldron_HandleInput(Cauldron* cauldron)
{
    //Insert ingredient to cauldron
    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && cauldron->selected_ingredient != NULL) {
        Vector2 mouse_pos = GetMousePosition();

        int grid_x = (mouse_pos.x - cauldron->active_grid_ptr->origin.x) / cauldron->active_grid_ptr->cell_size;
        int grid_y = (mouse_pos.y - cauldron->active_grid_ptr->origin.y) / cauldron->active_grid_ptr->cell_size;

        if(!Cauldron_InsertIngredient(cauldron, cauldron->selected_ingredient, grid_x, grid_y))
        {
            TraceLog(LOG_INFO, "Cannot insert ingredient at x:%i, y:%i", grid_x, grid_y);
        }
        else {
            TraceLog(LOG_INFO, "Inserted ingredient at x:%i, y:%i", grid_x, grid_y);
        }
    }


    // if(IsKeyDown(KEY_D))
    // {
    //     cauldron->active_grid.origin.x -= 1;
    //     cauldron->buffer_grid.origin.x -= 1;
    // }

    // if(IsKeyDown(KEY_A))
    // {
    //     cauldron->active_grid.origin.x += 1;
    //     cauldron->buffer_grid.origin.x += 1;
    // }

    // if(IsKeyDown(KEY_S))
    // {
    //     cauldron->active_grid.origin.y -= 1;
    //     cauldron->buffer_grid.origin.y -= 1;
    // }

    // if(IsKeyDown(KEY_W))
    // {
    //     cauldron->active_grid.origin.y += 1;
    //     cauldron->buffer_grid.origin.y += 1;
    // }

    // if(IsKeyPressed(KEY_SPACE))
    // {
    //     TraceLog(LOG_INFO, "Origin x:%.0f, y:%.0f", cauldron->active_grid.origin.x, cauldron->active_grid.origin.y);
    // }
}



void DrawCellInfo(const Cell* cell, const int screen_width, const int screen_height, struct nk_context* ctx)
{
    // Draw cell container
    int w = 250;
    int h = 100;
    int x = screen_width - w;
    int y = screen_height - h;
    DrawRectangle(x, y, w, h, LIGHTGRAY);

    char text[128];

    snprintf(text, sizeof(text), "CellType: %s, neighbors:", CellTypeToString(cell->type));



    if (nk_begin(ctx, "CellInfo", nk_rect(screen_width - w, screen_height - h, w, h), NK_WINDOW_BORDER)) {
        /* fixed widget pixel width */
        nk_layout_row_static(ctx, 30, 180, 1);
        nk_label(ctx, text, NK_TEXT_LEFT);
    }
    nk_end(ctx);
}

void Cauldron_DrawIngredientsBar(Cauldron* cauldron, IngredientList* ingredient_list, struct nk_context* ctx, int screen_height)
{
    if (nk_begin(ctx, "Add something to grid", nk_rect(20, screen_height - 148, 512, 128), NK_WINDOW_BORDER)) {
        /* fixed widget pixel width */
        nk_layout_row_dynamic(ctx, 24, 3);


        if (nk_button_label(ctx, "Salt"))
        {
            cauldron->selected_ingredient = Ingredient_Get(ingredient_list, INGREDIENT_SALT);
        }
    }
    nk_end(ctx);
}

void Cauldron_Destroy(Cauldron* cauldron)
{
    Grid_Destroy(&cauldron->active_grid);
    Grid_Destroy(&cauldron->buffer_grid);

    cauldron->active_grid_ptr = NULL;
    cauldron->buffer_grid_ptr = NULL;
}
