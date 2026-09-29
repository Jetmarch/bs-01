#include <assert.h>
#include <string.h>

#include "cauldron.h"
#include "cell.h"
#include "grid.h"
#include "ingredient.h"
#include "raylib.h"
#include "material.h"


typedef struct CellPair {
    Cell* active_cell;
    Cell* buffer_cell;
    bool is_valid;
} CellPair;

static bool Cauldron_InsertIngredient(Cauldron* cauldron, Ingredient* ingredient, int x, int y);
// static bool Cauldron_IsNeighbourExist(Grid* grid, int x, int y);
// static int Cauldron_GetNeighbourCount(Cauldron* brewing_grid, int x, int y);
// static void Update_B3S23_Rule(Cauldron* cauldron, int x, int y);
static void Update_Salt(Cauldron* cauldron, int x, int y);
static void Update_Gravity(Cauldron* cauldron, int x, int y);
static CellPair GetCellPair(Cauldron *cauldron, int x, int y);

// static const Direction DIRECTION_UP    = { 0, -1 };
static const Direction DIRECTION_DOWN  = { 0,  1 };
// static const Direction DIRECTION_LEFT  = {-1,  0 };
// static const Direction DIRECTION_RIGHT = { 1,  0 };



bool Cauldron_Init(Cauldron* cauldron, int grid_width, int grid_height, int screen_width, int screen_height, int cell_size, float duration_step_s)
{
    assert(cauldron != NULL);
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
            n_i =  y * grid_width + x;
            cell = &active_grid.cells[n_i];
            cell->material = Material_GetDefinition(MATERIAL_NONE);
            cell->y = y;
            cell->x = x;
            cell->rect.x = (x * cell_size);
            cell->rect.y = (y * cell_size);
            cell->rect.width = (float)cell_size;
            cell->rect.height = (float)cell_size;
            cell->is_selected = false;
        }
    }

    Grid buffer_grid;
    Grid_Init(&buffer_grid, grid_width, grid_height, grid_origin, cell_size);
    memcpy(buffer_grid.cells, active_grid.cells, sizeof(*active_grid.cells) * active_grid.count_of_cells);

    cauldron->active_grid = active_grid;
    cauldron->buffer_grid = buffer_grid;
    cauldron->selected_cell = NULL;
    cauldron->selected_ingredient = NULL;
    cauldron->is_brewing_in_process = false;
    cauldron->current_duration_ms = duration_step_s;
    cauldron->step_duration_ms = duration_step_s;

    cauldron->active_grid_ptr = &cauldron->active_grid;
    cauldron->buffer_grid_ptr = &cauldron->buffer_grid;
    cauldron->gravity = DIRECTION_DOWN;


    return true;
}

void Cauldron_SwapBuffers(Cauldron* cauldron)
{
    assert(cauldron != NULL);

    Grid* temp_grid = cauldron->active_grid_ptr;
    cauldron->active_grid_ptr = cauldron->buffer_grid_ptr;
    cauldron->buffer_grid_ptr = temp_grid;
}


void Cauldron_Update(Cauldron *cauldron, float delta)
{
    assert(cauldron != NULL);
    assert(delta > 0);
    assert(cauldron->active_grid.cells != NULL);
    assert(cauldron->buffer_grid.cells != NULL);


    if (!cauldron->is_brewing_in_process) {
        return;
    }

    cauldron->current_duration_ms -= delta;
    if(cauldron->current_duration_ms > 0.0) {
        return;
    }

    memcpy(cauldron->buffer_grid.cells,
            cauldron->active_grid.cells,
            cauldron->active_grid.count_of_cells * sizeof(*cauldron->active_grid.cells));

    for (int y = 0; y < cauldron->active_grid.height; y++)
    {
        for (int x = 0; x < cauldron->active_grid.width; x++)
        {
            // Update_B3S23_Rule(cauldron, x, y);
            //
            Update_Gravity(cauldron, x, y);
            Update_Salt(cauldron, x, y);
        }
    }

    cauldron->current_duration_ms = cauldron->step_duration_ms;

    Cauldron_SwapBuffers(cauldron);
}




void Cauldron_HandleInput(Cauldron* cauldron)
{
    assert(cauldron != NULL);

    //Insert ingredient to cauldron
    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && cauldron->selected_ingredient != NULL) {
        Vector2 mouse_pos = GetMousePosition();

        int grid_x = (mouse_pos.x + cauldron->active_grid_ptr->origin.x) / cauldron->active_grid_ptr->cell_size;
        int grid_y = (mouse_pos.y + cauldron->active_grid_ptr->origin.y) / cauldron->active_grid_ptr->cell_size;

        if(!Cauldron_InsertIngredient(cauldron, cauldron->selected_ingredient, grid_x, grid_y))
        {
            TraceLog(LOG_INFO, "Cannot insert ingredient at x:%i, y:%i", grid_x, grid_y);
        }
    }
}


void DrawCellInfo(const Cell* cell, const int screen_width, const int screen_height, struct nk_context* ctx)
{
    int w = 250;
    int h = 100;
    // int x = screen_width - w;
    // int y = screen_height - h;

    if(cell->amount > 1)
    {
        return;
    }


    if (nk_begin(ctx, "CellInfo", nk_rect(screen_width - w, screen_height - h, w, h), NK_WINDOW_BORDER)) {
        /* fixed widget pixel width */
        nk_layout_row_static(ctx, 30, 180, 1);
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
    assert(cauldron != NULL);

    Grid_Destroy(&cauldron->active_grid);
    Grid_Destroy(&cauldron->buffer_grid);

    cauldron->active_grid_ptr = NULL;
    cauldron->buffer_grid_ptr = NULL;
}

static bool Cauldron_InsertIngredient(Cauldron* cauldron, Ingredient* ingredient, int x, int y)
{
    assert(cauldron != NULL);
    assert(ingredient != NULL);

    return Grid_InsertGrid(&cauldron->active_grid, &ingredient->grid, x, y);
}


// static bool Cauldron_IsNeighbourExist(Grid* grid, int x, int y)
// {
//     Cell* cell = Grid_GetCellAtWrapAround(grid, x, y);

//     return cell->material == Material_GetDefinition(MATERIAL_NONE) ? 0 : 1;
// }

// static int Cauldron_GetNeighbourCount(Cauldron* brewing_grid, int x, int y)
// {
//     uint8_t neighbourCount = 0;

//     neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x - 1, y - 1);   // Top left
//     neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x, y - 1);       // Top middle
//     neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x + 1, y - 1);   // Top right
//     neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x - 1, y);       // Left
//     neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x + 1, y);       // Right
//     neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x - 1, y + 1);   // Bottom left
//     neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x, y + 1);       // Bottom middle
//     neighbourCount += Cauldron_IsNeighbourExist(brewing_grid->active_grid_ptr, x + 1, y + 1);

//     return neighbourCount;
// }

static CellPair GetCellPair(Cauldron *cauldron, int x, int y)
{
    CellPair pair = {
        .active_cell = Grid_GetCellAt(cauldron->active_grid_ptr, x, y),
        .buffer_cell = Grid_GetCellAt(cauldron->buffer_grid_ptr, x, y)
    };

    if (pair.buffer_cell == NULL || pair.active_cell == NULL)
    {
        TraceLog(LOG_ERROR, "Update_SaltCells: buffer_cell or active_cell is NULL");

        pair.is_valid = false;
        return pair;
    }

    pair.is_valid = true;
    return pair;
}

// static void Update_B3S23_Rule(Cauldron *cauldron, int x, int y)
// {
//     //TODO: Create system for every CellType
//     uint8_t neighbourCount = 0;
//     CellPair cell_pair = GetCellPair(cauldron, x, y);

//     if(!cell_pair.is_valid)
//     {
//         return;
//     }

//     neighbourCount = Cauldron_GetNeighbourCount(cauldron, x, y);

//     if (neighbourCount == 3) {
//         cell_pair.buffer_cell->material = Material_GetDefinition(MATERIAL_WATER);
//     }
//     else if (neighbourCount == 2) {
//         cell_pair.buffer_cell->material = cell_pair.active_cell->material;
//     }
//     else {
//         cell_pair.buffer_cell->material = Material_GetDefinition(MATERIAL_NONE);
//     }
// }

static void Update_Gravity(Cauldron* cauldron, int x, int y)
{
    CellPair cell_pair = GetCellPair(cauldron, x, y);

    if(!cell_pair.is_valid)
    {
        return;
    }

    int next_x = cell_pair.active_cell->x + cauldron->gravity.x;
    int next_y = cell_pair.active_cell->y + cauldron->gravity.y;

    CellPair next_cell_pair = GetCellPair(cauldron, next_x, next_y);

    if(!next_cell_pair.is_valid)
    {
        return;
    }

    Cell_CopyContent(cell_pair.active_cell, next_cell_pair.buffer_cell);
}

static void Update_Salt(Cauldron* cauldron, int x, int y)
{
    CellPair cell_pair = GetCellPair(cauldron, x, y);

    if(!cell_pair.is_valid)
    {
        return;
    }

    //Update gravity
    Cell* down_cell = Grid_GetCellAt(cauldron->active_grid_ptr, x, y + 1);
    if(down_cell == NULL)
    {
        return;
    }
}
