#define RAYLIB_NUKLEAR_IMPLEMENTATION

#include <stddef.h>

#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "rlgl.h"
#include "external/glad.h"


#include <raylib-nuklear/include/raylib-nuklear.h>




#define ENTITY_COUNT 10000

#define SCREEN_WIDTH 1240
#define SCREEN_HEIGHT 720

#define CELL_SIZE 100


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

typedef struct Button {
    Rectangle rect;
    Vector2 origin;
    float rotation;
    Color color;
} Button;

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


void DrawCellInfo(const Cell* cell, const int screen_width, const int screen_height)
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
}


void DrawCellButtons(Button* buttons, int size)
{
    for(int i = 0; i < size; i++)
    {
        Button* button = &buttons[i];

        if(CheckCollisionPointRec(GetMousePosition(), button->rect))
        {
            button->color = RED;
        }
        else
        {
            int color_modifier = i * 15;
            button->color = (Color){
                .r = 150 - color_modifier,
                .g = 150 - color_modifier,
                .b = 150 - color_modifier,
                .a = 255
            };
        }

        DrawRectanglePro(button->rect, button->origin, 0, button->color);
    }
}



int main(void)
{
    const int screen_width = SCREEN_WIDTH;
    const int screen_height = SCREEN_HEIGHT;
    const int max_cells = 9;
    const int grid_width = 3;
    const int grid_height = 3;

    const int font_size = 14;
    struct nk_context *ctx = InitNuklear(font_size);

    InitWindow(screen_width, screen_height, "bs-01");

    SetTargetFPS(60);

    Cell* cells = malloc(sizeof(Cell) * max_cells);

    int n_i = 0;
    Cell* cell = NULL;
    for (int y = 0; y < grid_height; y++)
    {
        for(int x = 0; x < grid_width; x++)
        {
            n_i =  y + grid_width * x;
            cell = &cells[n_i];

            if (n_i % 2 == 0) {
                cell->type = EMPTY_CELL;
            }
            else {
                cell->type = RED_CELL;
            }

            cell->rect.x = x * CELL_SIZE;
            cell->rect.y = y * CELL_SIZE;
            cell->rect.width = (float)CELL_SIZE;
            cell->rect.height = (float)CELL_SIZE;

            // TraceLog(LOG_INFO, "Cell x: %.0f y: %.0f, w: %.0f, h: %.0f type: %i", cell->rect.x, cell->rect.y, cell->rect.width, cell->rect.height, cell->type);
        }
    }

    Grid grid = {
        .width = grid_width,
        .height = grid_height,
        .count_of_cells = grid_width * grid_height,
        .origin = {0, 0},
        .cells = cells
    };

    Cell* currentCell = NULL;

    float button_size = 150;
    Button buttons[CELL_TYPE_COUNT];

    for(int i = 0; i < CELL_TYPE_COUNT; i++)
    {
        Button* button = &buttons[i];
        button->rect = (Rectangle) {
            .x = (button_size + 20) * i,
            .y = screen_height - 170,
            .width = button_size,
            .height = button_size
        };
        button->rotation = 0;
        button->origin.y = 0;
        button->origin.x = 0;

        char text[64];

        snprintf(text, sizeof(text), "Button x: %.0f, y: %.0f, w: %.0f, h: %.0f", button->rect.x, button->rect.y, button->rect.width, button->rect.height);

        TraceLog(LOG_INFO, text);

        snprintf(text, sizeof(text), "Origin x: %.0f, y: %.0f", button->origin.x, button->origin.y);

        TraceLog(LOG_INFO, text);
    }

    while(!WindowShouldClose())
    {
        UpdateNuklear(ctx);

        Vector2 mouse_pos = GetMousePosition();

        Cell* cell = NULL;
        for (int i = 0; i < grid.count_of_cells; i++)
        {
            cell = &grid.cells[i];
            if (CheckCollisionPointRec(mouse_pos, cell->rect))
            {
                cell->is_selected = true;

                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    TraceLog(LOG_INFO, "Cell %i was clicked", cell->type);
                    currentCell = cell;
                }
            }
            else {
                cell->is_selected = false;
            }
        }

        if (nk_begin(ctx, "Nuklear", nk_rect(100, 100, 220, 220),
                NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
            nk_layout_row_static(ctx, 50, 150, 1);
            if (nk_button_label(ctx, "Button")) {
                // Button was clicked!
            }
        }
        nk_end(ctx);


        // There will be logic for updating the grid by the rules of the game of life



        BeginDrawing();

        ClearBackground(BLUE);

        Draw2DGrid(&grid);

        if(currentCell != NULL)
        {
            DrawCellInfo(currentCell, screen_width, screen_height);
            DrawCellButtons(buttons, CELL_TYPE_COUNT);
        }

        DrawNuklear(ctx);

        EndDrawing();
    }

    free(cells);

    UnloadNuklear(ctx);

    CloseWindow();

    return 0;
}
