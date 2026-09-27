#include "ingredient.h"
#include "cell.h"
#include "grid.h"
#include <stdlib.h>


bool Ingredient_InitList(IngredientList* ingredient_list)
{
    ingredient_list->list = calloc(INGREDIENT_TYPE_COUNT, sizeof *ingredient_list->list);

    if (ingredient_list->list == NULL)
    {
        return false;
    }

    Grid salt_grid = {0};
    if (!Grid_Init(&salt_grid, 3, 4, (Vector2){0, 0}, 30))
    {
        TraceLog(LOG_FATAL, "Grid_Init: Cannot create INGREDIENT_SALT grid");
        return false;
    }

    Cell* cell = Grid_GetCellAt(&salt_grid, 0, 0);
    if(cell == NULL)
    {
        TraceLog(LOG_FATAL, "Grid_GetCellAt: Cannot get cell at {0, 0}");
        return false;
    }

    cell->type = BLUE_CELL;

    ingredient_list->list[0] = (Ingredient) {
        .type = INGREDIENT_SALT,
        .grid = salt_grid
    };

    return true;
}

Ingredient* Ingredient_Get(IngredientList* ingredient_list, IngredientType type)
{
    for (int i = 0; i < INGREDIENT_TYPE_COUNT; i++)
    {
        if (ingredient_list->list[i].type == type)
        {
            return &ingredient_list->list[i];
        }
    }

    return NULL;
}

void Ingredient_FreeList(IngredientList* ingredient_list)
{
    free(ingredient_list->list);
    ingredient_list->list = NULL;
}
