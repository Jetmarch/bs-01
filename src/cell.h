#ifndef CELL_H
#define CELL_H

#include <raylib.h>
#include <raylib-nuklear/include/raylib-nuklear.h>
#include "material.h"
#include "utils.h"

#define CELL_MAX_AMOUNT 100.0f

enum CellType
{
    EMPTY_CELL,
    RED_CELL,
    BLUE_CELL,
    GREEN_CELL,
    YELLOW_CELL,

    SALT_CELL,

    CELL_TYPE_COUNT
};

typedef struct Cell
{
    int x, y;

    Rectangle rect;
    bool is_selected;

    const MaterialDefinition *material;

    float amount;
    float temperature;
} Cell;

Color SolveCellColor(const Cell *cell);
void Cell_MoveContent(Cell *source, Cell *target);
void Cell_CopyContent(Cell *source, Cell *target);
void Cell_CleanContent(Cell *target);

#endif
