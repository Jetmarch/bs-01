#include "cell.h"

#include <stdio.h>

Color SolveCellColor(const Cell* cell)
{
    if (cell->is_selected)
    {
        return LIGHTGRAY;
    }

    switch (cell->type)
    {
        case EMPTY_CELL:
            return DARKGRAY;
        case RED_CELL:
           return RED;
        case BLUE_CELL:
            return BLUE;
        case GREEN_CELL:
            return GREEN;
        case YELLOW_CELL:
            return YELLOW;
        case CELL_TYPE_COUNT:
            return LIGHTGRAY;
    }
}

const char *CellTypeToString(enum CellType type)
{
    switch (type)
    {
        case EMPTY_CELL:  return "empty";
        case RED_CELL:    return "red";
        case BLUE_CELL:   return "blue";
        case GREEN_CELL:  return "green";
        case YELLOW_CELL: return "yellow";
        default:          return "unknown";
    }
}

void PrintCell(const Cell* cell)
{
    printf(
        "Cell {\n"
        "    x=%i, y=%i\n"
        "    rect: x=%.2f, y=%.2f, width=%.2f, height=%.2f\n"
        "    is_selected: %s\n"
        "    type: %d\n"
        "}\n",
        cell->x,
        cell->y,
        cell->rect.x,
        cell->rect.y,
        cell->rect.width,
        cell->rect.height,
        cell->is_selected ? "true" : "false",
        cell->type
    );
}
