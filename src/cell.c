#include "cell.h"
#include "raylib.h"


Color SolveCellColor(const Cell* cell)
{
    if(cell->is_selected)
    {
        return VIOLET;
    }

    switch (cell->type)
    {
        case EMPTY_CELL:
            return DARKGRAY;
        case SALT_CELL:
           return LIGHTGRAY;
        default:
            return VIOLET;
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
