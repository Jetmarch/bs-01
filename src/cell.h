#include <raylib.h>
#include <raylib-nuklear/include/raylib-nuklear.h>

enum CellType {
    EMPTY_CELL,
    RED_CELL,
    BLUE_CELL,
    GREEN_CELL,
    YELLOW_CELL,

    CELL_TYPE_COUNT
};


typedef struct Cell {
    int x, y;
    Rectangle rect;
    bool is_selected;
    enum CellType type;
} Cell;





Color SolveCellColor(const Cell* cell);
const char *CellTypeToString(enum CellType type);

// void DrawCellInfo(const Cell* cell, const int screen_width, const int screen_height, struct nk_context* ctx);
void DrawCellButtons(struct nk_context* ctx, int screen_height, Cell* current_cell);

void PrintCell(const Cell* cell);
