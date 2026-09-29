#include "ingredient.h"
#include "cell.h"
#include "grid.h"
#include <stdlib.h>

typedef struct IngredientDefinition {
    enum IngredientType type;

    int width;
    int height;

    const enum CellType* cells;
} IngredientDefinition;

static const enum CellType SALT_CELLS[3 * 3] = {
    EMPTY_CELL, SALT_CELL,  EMPTY_CELL,
    SALT_CELL,  SALT_CELL,  SALT_CELL,
    EMPTY_CELL, SALT_CELL,  EMPTY_CELL
};

static const IngredientDefinition INGREDIENT_DEFINITIONS[] = {
    [INGREDIENT_SALT] = {
        .type = INGREDIENT_SALT,
        .width = 3,
        .height = 3,
        .cells = SALT_CELLS
    }
};

static bool Ingredient_Init(Ingredient* ingredient, const IngredientDefinition* definition);

bool Ingredient_InitList(IngredientList* ingredient_list)
{
    ingredient_list->list = calloc(INGREDIENT_TYPE_COUNT, sizeof *ingredient_list->list);

    if (ingredient_list->list == NULL)
    {
        return false;
    }

    for (int i = 0; i < INGREDIENT_TYPE_COUNT; ++i)
    {
        if (!Ingredient_Init(
                &ingredient_list->list[i],
                &INGREDIENT_DEFINITIONS[i]))
        {
            Ingredient_FreeList(ingredient_list);
            return false;
        }
    }

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

static bool Ingredient_Init(
    Ingredient* ingredient,
    const IngredientDefinition* definition)
{
    ingredient->type = definition->type;

    if (!Grid_Init(
            &ingredient->grid,
            definition->width,
            definition->height,
            (Vector2){0, 0},
            30))
    {
        return false;
    }

    for (int y = 0; y < definition->height; ++y)
    {
        for (int x = 0; x < definition->width; ++x)
        {
            Cell* cell = Grid_GetCellAt(
                &ingredient->grid,
                x,
                y
            );

            cell->type =
                definition->cells[y * definition->width + x];
        }
    }

    return true;
}
