#version 430

// raylib [rlgl] compute shader example - Game of Life
// "Render" pass: fragment shader, paired with raylib's built-in default vertex
// shader (pass NULL as vertex shader code to LoadShaderFromMemory). Draws a
// full-screen white texture through this shader; each fragment looks up its
// cell state directly from the SSBO and outputs black/white accordingly.

in vec2 fragTexCoord;   // provided by raylib's default vertex shader
in vec4 fragColor;      // provided by raylib's default vertex shader

out vec4 finalColor;

#define GOL_WIDTH 768

layout(std430, binding = 1) readonly buffer golBufferA
{
    uint golBufferAiv[];
};

uniform vec2 resolution; // set from C/Odin: { GOL_WIDTH, GOL_WIDTH }

void main()
{
    ivec2 coords = ivec2(fragTexCoord * resolution);
    coords = clamp(coords, ivec2(0), ivec2(GOL_WIDTH - 1));

    uint cell = golBufferAiv[coords.y*GOL_WIDTH + coords.x];

    finalColor = (cell == 1u) ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
}
