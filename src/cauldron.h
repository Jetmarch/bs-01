#ifndef CAULDRON_H
#define CAULDRON_H

#include "grid.h"
#include "ingredient.h"


typedef struct Cauldron {
    Grid active_grid;
    Grid buffer_grid;
    bool is_brewing_in_process;
    float step_duration_ms;
    float current_duration_ms;

    Cell* selected_cell;

    Grid* active_grid_ptr;
    Grid* buffer_grid_ptr;

    Ingredient* selected_ingredient;
} Cauldron;


bool Cauldron_Init(Cauldron* cauldron, int grid_width, int grid_height, int screen_width, int screen_height, int cell_size, float duration_step_s);
void Cauldron_Update(Cauldron* cauldron, float delta);
void Cauldron_HandleInput(Cauldron* cauldron);
void Cauldron_Destroy(Cauldron* cauldron);
void Cauldron_DrawIngredientsBar(Cauldron* cauldron, IngredientList* ingredient_list, struct nk_context* ctx, int screen_height);
void DrawCellInfo(const Cell* cell, const int screen_width, const int screen_height, struct nk_context* ctx);

#endif
