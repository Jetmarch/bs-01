#version 430

// raylib [rlgl] compute shader example - Game of Life
// "Logic" pass: reads current generation (bufferA) and writes the next one (bufferB)
//
// IMPORTANT: GOL_WIDTH must match the constant used on the C/Odin side,
// and must stay a multiple of 16 (matches local_size_x/y below).

#define GOL_WIDTH 768

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout(std430, binding = 1) readonly buffer golBufferA
{
    uint golBufferAiv[];
};

layout(std430, binding = 2) writeonly buffer golBufferB
{
    uint golBufferBiv[];
};

// Toroidal (wrap-around) lookup so gliders can cross the screen edges
int GetCellAt(int x, int y)
{
    x = (x + GOL_WIDTH) % GOL_WIDTH;
    y = (y + GOL_WIDTH) % GOL_WIDTH;
    return int(golBufferAiv[y*GOL_WIDTH + x]);
}

void main()
{
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
    if (coords.x >= GOL_WIDTH || coords.y >= GOL_WIDTH) return;

    int neighbours = 0;
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0) continue;
            neighbours += GetCellAt(coords.x + dx, coords.y + dy);
        }
    }

    int current = GetCellAt(coords.x, coords.y);
    int next = 0;

    if (current == 1)
    {
        // Any live cell with 2 or 3 live neighbours survives
        if (neighbours == 2 || neighbours == 3) next = 1;
    }
    else
    {
        // Any dead cell with exactly 3 live neighbours becomes alive
        if (neighbours == 3) next = 1;
    }

    golBufferBiv[coords.y*GOL_WIDTH + coords.x] = uint(next);
}
