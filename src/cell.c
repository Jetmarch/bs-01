#include "cell.h"
#include "material.h"
#include "raylib.h"
#include <assert.h>

Color SolveCellColor(const Cell *cell)
{
    assert(cell != NULL);
    if (cell->is_selected)
    {
        return VIOLET;
    }

    if (cell->material == Material_GetDefinition(MATERIAL_NONE))
    {
        return DARKGRAY;
    }

    switch (cell->material->type)
    {
    case MATERIAL_WATER:
        return (Color){45, 105, 155, 255}; // deep blue
    case MATERIAL_SALT:
        return (Color){205, 200, 185, 255}; // warm white
    case MATERIAL_IRON:
        return (Color){95, 100, 105, 255}; // cold metal
    case MATERIAL_RUST:
        return (Color){155, 75, 35, 255}; // oxidized iron
    default:
        return (Color){190, 110, 220, 255}; // unknown
    }
}

void Cell_MoveContent(Cell *source, Cell *target)
{
    target->material = source->material;
    target->amount = source->amount;
    target->temperature = source->temperature;

    source->material = NULL;
    source->amount = 0.0f;
    source->temperature = 0.0f;
}

void Cell_CopyContent(Cell *source, Cell *target)
{
    target->material = source->material;
    target->amount = source->amount;
    target->temperature = source->temperature;
}

void Cell_CleanContent(Cell *target)
{
    target->material = Material_GetDefinition(MATERIAL_NONE);
    target->amount = 0;
    target->temperature = 0;
}
