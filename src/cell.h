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
    Rectangle rect;
    bool is_selected;
    enum CellType type;
} Cell;

typedef struct Grid {
    int width;
    int height;
    int count_of_cells;
    Vector2 origin;
    Cell* cells;
} Grid;


Cell* GetCellAt(Grid* grid, int x, int y);
Cell* GetCellAtWrapAround(Grid* grid, int x, int y);
Color SolveCellColor(const Cell* cell);
const char *CellTypeToString(enum CellType type);
void Draw2DGrid(Grid* grid);
void DrawCellInfo(const Cell* cell, const int screen_width, const int screen_height, struct nk_context* ctx);
void DrawCellButtons(struct nk_context* ctx, int screen_height, Cell* current_cell);
