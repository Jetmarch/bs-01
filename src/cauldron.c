#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "cauldron.h"
#include "cell.h"
#include "grid.h"
#include "ingredient.h"
#include "raylib.h"
#include "material.h"

typedef struct CellPair
{
    Cell *active_cell;
    Cell *buffer_cell;
    bool is_valid;
} CellPair;

static bool Cauldron_InsertIngredient(Cauldron *cauldron, Ingredient *ingredient, int x, int y);
static void Update_Flow(Cauldron *cauldron, int x, int y);
static void Update_Gravity(Cauldron *cauldron, int x, int y);
static CellPair GetCellPair(Cauldron *cauldron, int x, int y);

static const Direction DIRECTION_DOWN = {.x = 0, .y = 1};

bool Cauldron_Init(Cauldron *cauldron, int grid_width, int grid_height, int screen_width, int screen_height, int cell_size, float duration_step_s)
{
    assert(cauldron != NULL);
    Vector2 grid_origin = {
        -(((float)screen_width / 2) - ((float)grid_width * cell_size) / 2),
        -(((float)screen_height / 2) - ((float)grid_height * cell_size) / 2)};

    Grid active_grid = {0};
    if (!Grid_Init(&active_grid, grid_width, grid_height, grid_origin, cell_size))
    {
        TraceLog(LOG_ERROR, "Active grid not initialized. Aborting");
        return false;
    }

    int n_i = 0;
    Cell *cell = NULL;
    for (int y = 0; y < grid_height; y++)
    {
        for (int x = 0; x < grid_width; x++)
        {
            n_i = y * grid_width + x;
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

void Cauldron_SwapBuffers(Cauldron *cauldron)
{
    assert(cauldron != NULL);
    assert(cauldron->active_grid_ptr != NULL);
    assert(cauldron->buffer_grid_ptr != NULL);

    Grid *temp_grid = cauldron->active_grid_ptr;
    cauldron->active_grid_ptr = cauldron->buffer_grid_ptr;
    cauldron->buffer_grid_ptr = temp_grid;

    if (cauldron->active_grid_ptr == &cauldron->active_grid)
    {
        TraceLog(LOG_INFO, "Active grid, buffer grid");
    }
    else if (cauldron->active_grid_ptr == &cauldron->buffer_grid)
    {
        TraceLog(LOG_INFO, "Buffer grid, active grid");
    }
    else
    {
        TraceLog(LOG_INFO, "Unknow state of buffers");
    }
}

void Cauldron_Update(Cauldron *cauldron, float delta)
{

    assert(cauldron != NULL);
    // assert(delta > 0);
    assert(cauldron->active_grid_ptr != NULL);
    assert(cauldron->buffer_grid_ptr != NULL);
    assert(cauldron->active_grid.cells != NULL);
    assert(cauldron->buffer_grid.cells != NULL);

    if (!cauldron->is_brewing_in_process)
    {
        return;
    }

    cauldron->current_duration_ms -= delta;
    if (cauldron->current_duration_ms > 0.0)
    {
        return;
    }

    TraceLog(LOG_INFO, "-----------------------------------");

    cauldron->active_grid_ptr->count_of_active_materials = 0;
    cauldron->buffer_grid_ptr->count_of_active_materials = 0;

    for (int i = 0; i < cauldron->active_grid_ptr->count_of_cells; i++)
    {
        cauldron->buffer_grid_ptr->cells[i].amount = 0;
        cauldron->buffer_grid_ptr->cells[i].material = NULL;
        cauldron->buffer_grid_ptr->cells[i].temperature = 0;
    }

    // Upside-down cells process
    for (int y = 0; y < cauldron->active_grid_ptr->height; y++)
    {
        for (int x = 0; x < cauldron->active_grid_ptr->width; x++)
        {
            Cell *cell = Grid_GetCellAt(cauldron->active_grid_ptr, x, y);

            if (cell->material == Material_GetDefinition(MATERIAL_NONE))
            {
                continue;
            }
            cauldron->active_grid_ptr->count_of_active_materials++;
        }
    }

    // Downside-up cells process
    for (int y = cauldron->active_grid_ptr->height - 1; y >= 0; y--)
    {
        for (int x = cauldron->active_grid_ptr->width - 1; x >= 0; x--)
        {
            Cell *cell = Grid_GetCellAt(cauldron->active_grid_ptr, x, y);

            if (cell->material == Material_GetDefinition(MATERIAL_NONE))
            {
                continue;
            }

            if (cell->material->gravity_strength > 0.0f)
            {
                Update_Gravity(cauldron, x, y);
            }

            if (cell->material->is_liquid)
            {
                Update_Flow(cauldron, x, y);
            }
        }
    }

    for (int y = 0; y < cauldron->buffer_grid_ptr->height; y++)
    {
        for (int x = 0; x < cauldron->buffer_grid_ptr->width; x++)
        {
            Cell *cell = Grid_GetCellAt(cauldron->buffer_grid_ptr, x, y);

            if (cell->material == Material_GetDefinition(MATERIAL_NONE))
            {
                continue;
            }
            cauldron->buffer_grid_ptr->count_of_active_materials++;
        }
    }

    cauldron->current_duration_ms = cauldron->step_duration_ms;

    Cauldron_SwapBuffers(cauldron);
    TraceLog(LOG_INFO, "Active grid items: %i, buffer grid items: %i",
             cauldron->active_grid_ptr->count_of_active_materials,
             cauldron->buffer_grid_ptr->count_of_active_materials);
    TraceLog(LOG_INFO, "-----------------------------------");
}

void Cauldron_HandleInput(Cauldron *cauldron)
{
    assert(cauldron != NULL);

    // Insert ingredient to cauldron
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && cauldron->selected_ingredient != NULL)
    {
        TraceLog(LOG_INFO, "Trying to insert ingredient into cauldron");
        Vector2 mouse_pos = GetMousePosition();

        int grid_x = (mouse_pos.x + cauldron->active_grid_ptr->origin.x) / cauldron->active_grid_ptr->cell_size;
        int grid_y = (mouse_pos.y + cauldron->active_grid_ptr->origin.y) / cauldron->active_grid_ptr->cell_size;

        if (!Cauldron_InsertIngredient(cauldron, cauldron->selected_ingredient, grid_x, grid_y))
        {
            TraceLog(LOG_INFO, "Cannot insert ingredient at x:%i, y:%i", grid_x, grid_y);
        }
        else
        {
            TraceLog(LOG_INFO, "Cannot insert ingredient at x:%i, y:%i", grid_x, grid_y);
        }
    }
}

void DrawCellInfo(const Cell *cell, const int screen_width, const int screen_height, struct nk_context *ctx)
{
    int w = 250;
    int h = 100;
    // int x = screen_width - w;
    // int y = screen_height - h;

    if (cell->amount > 1)
    {
        return;
    }

    if (nk_begin(ctx, "CellInfo", nk_rect(screen_width - w, screen_height - h, w, h), NK_WINDOW_BORDER))
    {
        /* fixed widget pixel width */
        nk_layout_row_static(ctx, 30, 180, 1);
    }
    nk_end(ctx);
}

void Cauldron_DrawIngredientsBar(Cauldron *cauldron, IngredientList *ingredient_list, struct nk_context *ctx, int screen_height)
{
    if (nk_begin(ctx, "Add something to grid", nk_rect(20, screen_height - 148, 256, 128), NK_WINDOW_BORDER))
    {
        /* fixed widget pixel width */
        nk_layout_row_dynamic(ctx, 24, 3);

        if (nk_button_label(ctx, "Salt"))
        {
            cauldron->selected_ingredient = Ingredient_Get(ingredient_list, INGREDIENT_SALT);
        }

        if (nk_button_label(ctx, "Water"))
        {
            cauldron->selected_ingredient = Ingredient_Get(ingredient_list, INGREDIENT_WATER);
        }

        if (nk_button_label(ctx, "Iron"))
        {
            cauldron->selected_ingredient = Ingredient_Get(ingredient_list, INGREDIENT_IRON);
        }
    }
    nk_end(ctx);
}

void Cauldron_Destroy(Cauldron *cauldron)
{
    assert(cauldron != NULL);

    Grid_Destroy(&cauldron->active_grid);
    Grid_Destroy(&cauldron->buffer_grid);

    cauldron->active_grid_ptr = NULL;
    cauldron->buffer_grid_ptr = NULL;
}

static bool Cauldron_InsertIngredient(Cauldron *cauldron, Ingredient *ingredient, int x, int y)
{
    assert(cauldron != NULL);
    assert(ingredient != NULL);

    return Grid_InsertGrid(cauldron->active_grid_ptr, &ingredient->grid, x, y);
}

static CellPair GetCellPair(Cauldron *cauldron, int x, int y)
{
    assert(cauldron != NULL);

    CellPair pair = {
        .active_cell = Grid_GetCellAt(cauldron->active_grid_ptr, x, y),
        .buffer_cell = Grid_GetCellAt(cauldron->buffer_grid_ptr, x, y)};

    if (pair.buffer_cell == NULL || pair.active_cell == NULL)
    {
        TraceLog(LOG_ERROR, "GetCellPair: buffer_cell or active_cell is NULL");

        pair.is_valid = false;
        return pair;
    }

    pair.is_valid = true;
    return pair;
}

static bool Try_Move_Cell(Cauldron *cauldron, int from_x, int from_y, int to_x, int to_y)
{
    CellPair cell = GetCellPair(cauldron, from_x, from_y);
    CellPair next_cell = GetCellPair(cauldron, to_x, to_y);

    if (next_cell.buffer_cell == NULL)
    {
        Cell_CopyContent(cell.active_cell, Grid_GetCellAt(cauldron->buffer_grid_ptr, from_x, from_y));
        return false;
    }

    if (next_cell.buffer_cell->material != NULL)
    {
        Cell_CopyContent(cell.active_cell, Grid_GetCellAt(cauldron->buffer_grid_ptr, from_x, from_y));
        return false;
    }

    Cell_CopyContent(cell.active_cell, next_cell.buffer_cell);

    return true;
}

static void Update_Gravity(Cauldron *cauldron, int x, int y)
{

    CellPair cell = GetCellPair(cauldron, x, y);

    if (!cell.is_valid)
    {
        return;
    }

    int next_x = ceil(cell.active_cell->x + cauldron->gravity.x * cell.active_cell->material->gravity_strength);
    int next_y = ceil(cell.active_cell->y + cauldron->gravity.y * cell.active_cell->material->gravity_strength);
    next_x = Clamp(next_x, 0, cauldron->active_grid_ptr->width);
    next_y = Clamp(next_y, 0, cauldron->active_grid_ptr->height);

    if (Try_Move_Cell(cauldron, x, y, next_x, next_y))
    {
        return;
    }

    // Trying to place cell in a previous positions until it not reaches initial position
    while (!Try_Move_Cell(cauldron, x, y, next_x, next_y))
    {
        next_x = next_x - cauldron->gravity.x;
        next_y = next_y - cauldron->gravity.y;

        if (next_x < x || next_y < y)
        {
            break;
        }
    }
}

static void Update_Flow(Cauldron *cauldron, int x, int y)
{
    // try_down();

    // if (!moved)
    // {
    //     try_down_left();
    // }

    // if (!moved)
    // {
    //     try_down_right();
    // }

    // if (!moved)
    // {
    //     try_left();
    // }

    // if (!moved)
    // {
    //     try_right();
    // }
}
