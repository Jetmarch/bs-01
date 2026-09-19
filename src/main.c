#include <stddef.h>

#include <raylib.h>
#include <stdio.h>
#include "rlgl.h"
#include "external/glad.h"

#define ENTITY_COUNT 10000


static const char *compute_shader_path = "assets/shaders/move.comp";


typedef struct
{
    float x;
    float y;
} Position;

typedef void (*GLGenBuffersProc)(
    int n,
    unsigned int *buffers
);

typedef void (*GLBindBufferProc)(
    unsigned int target,
    unsigned int buffer
);

typedef void (*GLBufferDataProc)(
    unsigned int target,
    long long size,
    const void *data,
    unsigned int usage
);

typedef GLuint (*GLCreateShaderProc)(GLenum type);
typedef void (*GLShaderSourceProc)(
    GLuint shader,
    GLsizei count,
    const char **string,
    const GLint *length
);
typedef void (*GLCompileShaderProc)(GLuint shader);
typedef void (*GLGetShaderivProc)(
    GLuint shader,
    GLenum pname,
    GLint *params
);
typedef void (*GLGetShaderInfoLogProc)(
    GLuint shader,
    GLsizei maxLength,
    GLsizei *length,
    char *infoLog
);

typedef GLuint (*GLCreateProgramProc)(void);
typedef void (*GLAttachShaderProc)(
    GLuint program,
    GLuint shader
);
typedef void (*GLLinkProgramProc)(GLuint program);
typedef void (*GLGetProgramivProc)(
    GLuint program,
    GLenum pname,
    GLint *params
);
typedef void (*GLGetProgramInfoLogProc)(
    GLuint program,
    GLsizei maxLength,
    GLsizei *length,
    char *infoLog
);
typedef void (*GLUseProgramProc)(GLuint program);



int main(void)
{

    const int screen_width = 800;
    const int screen_height = 450;

    InitWindow(screen_width, screen_height, "bs-01");

    int groups = (ENTITY_COUNT + 255) / 256;

    SetTargetFPS(60);

    Position positions[ENTITY_COUNT];

    for(int i = 0; i < ENTITY_COUNT; i++)
    {
        positions[i].x = (float)i;
        positions[i].y = 0.0f;
    }


    if(!glGenBuffers || !glBindBuffer || !glBufferData)
    {
        TraceLog(LOG_ERROR, "Failed to load OpenGL functions");
        CloseWindow();
        return 1;
    }

    unsigned int ssbo;

    glad_glGenBuffers(1, &ssbo);

    glad_glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

    glad_glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);

    // Shader shader = LoadShader(NULL, fragment_shader_path);
    //

    printf("Size: %zu bytes\n", sizeof(positions));

    while(!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        // BeginShaderMode(shader);

        // DrawRectangle(300, 150, 200, 150, RED);

        // EndShaderMode();


        DrawText("SSBO created", 20, 20, 30, WHITE);

        EndDrawing();
    }

    // UnloadShader(shader);

    CloseWindow();

    return 0;
}
