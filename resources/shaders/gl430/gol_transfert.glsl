#version 430

// raylib [rlgl] compute shader example - Game of Life
// "Transfert" pass: applies buffered mouse-draw commands directly into bufferA,
// instead of stepping the simulation. Run this pass on the frame(s) where the
// user is drawing, and run gol_logic.glsl on the frames where they are not.

#define GOL_WIDTH 768
#define MAX_BUFFERED_TRANSFERTS 48

// Must mirror the C/Odin struct:
// typedef struct GolUpdateCmd { unsigned int x, y, w, enabled; } GolUpdateCmd;
struct GolUpdateCmd
{
    uint x;         // top-left X of the brush square
    uint y;         // top-left Y of the brush square
    uint w;         // brush square size (width == height)
    uint enabled;   // 1 = set cells alive, 0 = set cells dead
};

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 1) buffer golBufferA
{
    uint golBufferAiv[];
};

// Must mirror the C/Odin struct:
// typedef struct GolUpdateSSBO { unsigned int count; GolUpdateCmd commands[MAX_BUFFERED_TRANSFERTS]; } GolUpdateSSBO;
layout(std430, binding = 3) readonly buffer golUpdateCmds
{
    uint count;
    GolUpdateCmd commands[MAX_BUFFERED_TRANSFERTS];
} transfertBuffer;

void main()
{
    // Single work-item: dispatch this shader with rlComputeShaderDispatch(1, 1, 1)
    for (uint i = 0u; i < transfertBuffer.count; i++)
    {
        GolUpdateCmd cmd = transfertBuffer.commands[i];

        for (uint yy = 0u; yy < cmd.w; yy++)
        {
            for (uint xx = 0u; xx < cmd.w; xx++)
            {
                uint px = cmd.x + xx;
                uint py = cmd.y + yy;

                if (px < GOL_WIDTH && py < GOL_WIDTH)
                {
                    golBufferAiv[py*GOL_WIDTH + px] = cmd.enabled;
                }
            }
        }
    }
}
