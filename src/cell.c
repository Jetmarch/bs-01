#include "cell.h"


#include <stdio.h>

Cell* GetCellAt(Grid* grid, int x, int y)
{
    return &grid->cells[y + grid->width + x];
}

//Wrap around lookup
Cell* GetCellAtWrapAround(Grid* grid, int x, int y)
{
    int t_x = (x + grid->width) % grid->width;
    int t_y = (y + grid->height) % grid->height;

    return GetCellAt(grid, t_x, t_y);
}

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

void Draw2DGrid(Grid* grid)
{
    for(int i = 0; i < grid->count_of_cells; i++)
    {
        Cell* cell = &grid->cells[i];
        DrawRectanglePro(cell->rect, grid->origin, 0.0, SolveCellColor(cell));
    }
}

void DrawCellInfo(const Cell* cell, const int screen_width, const int screen_height, struct nk_context* ctx)
{
    // Draw cell container
    int w = 250;
    int h = 100;
    int x = screen_width - w;
    int y = screen_height - h;
    DrawRectangle(x, y, w, h, LIGHTGRAY);

    char text[64];

    snprintf(
        text,
        sizeof(text),
        "Cell is %s",
        CellTypeToString(cell->type)
    );

    int text_size = MeasureText(text, 20);
    DrawText(text, x + text_size - 50, y + 35, 20, BLACK);


    if (nk_begin(ctx, "CellInfo", nk_rect(screen_width - w, screen_height - h, w, h), NK_WINDOW_BORDER)) {
        /* fixed widget pixel width */
        nk_layout_row_static(ctx, 30, 80, 1);


        nk_label(ctx, text, NK_TEXT_LEFT);
    }
    nk_end(ctx);

}

void DrawCellButtons(struct nk_context* ctx, int screen_height, Cell* current_cell)
{
    if (nk_begin(ctx, "Add something to grid", nk_rect(20, screen_height - 148, 512, 128), NK_WINDOW_BORDER)) {
        /* fixed widget pixel width */
        nk_layout_row_dynamic(ctx, 24, 3);


        if (nk_button_label(ctx, "Red ingredient"))
        {
            current_cell->type = RED_CELL;
        }

        if (nk_button_label(ctx, "Blue ingredient"))
        {
            current_cell->type = BLUE_CELL;
        }

        if (nk_button_label(ctx, "Green ingredient"))
        {
            current_cell->type = GREEN_CELL;
        }

        if (nk_button_label(ctx, "Yellow ingredient"))
        {
            current_cell->type = YELLOW_CELL;
        }

        if (nk_button_label(ctx, "Empty cell"))
        {
            current_cell->type = EMPTY_CELL;
        }
    }
    nk_end(ctx);
}
