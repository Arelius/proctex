#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#include <stdio.h>
#include <stdlib.h>

#define GL_DECL(fn, dcl) dcl fn = nullptr;
#include "glext_loader.inc"
#undef GL_DECL

void LoadGLExtensions()
{
#define GL_DECL(fn, dcl) fn = (dcl)SDL_GL_GetProcAddress(#fn);
#include "glext_loader.inc"
#undef GL_DECL
}

GLuint CreateShaderProgram(const char* vfilename, const char* pfilename);
void DestroyProgramAndAttachedShaders(GLuint program);

GLuint gQuad;

struct tex_quad_vert
{
    float location[3];
    float uv[2];
};

const tex_quad_vert quad_mesh[] =
{{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
 {{1.0f, -1.0f, 0.0f},  {1.0f, 0.0f}},
 {{-1.0f, 1.0f, 0.0f},  {0.0f, 1.0f}},
 {{-1.0f, 1.0f, 0.0f},  {0.0f, 1.0f}},
 {{1.0f, -1.0f, 0.0f},  {1.0f, 0.0f}},
 {{1.0f, 1.0f, 0.0f},   {1.0f, 1.0f}}};

void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user)
{
    printf("OpenGL Error\n");
}

int SDL_main(int argc, char* argv[])
{

    if(argc != 3)
    {
        printf("Expecting vertex and pixel shader paths.\n");
        return -1;
    }

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return -1;
    }

    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GLcontextFlag::SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_Window* window;
    window = SDL_CreateWindow("proctex", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 1024, SDL_WINDOW_OPENGL);

    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_GLContext context = SDL_GL_CreateContext(window);

    SDL_GL_MakeCurrent(window, context);

    LoadGLExtensions();

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugCallback, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    //glViewport(0, 0, 1024, 1024);
    //glShadeModel(GL_SMOOTH);
    //glEnable(GL_BLEND);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);

    GLuint shader = CreateShaderProgram(argv[1], argv[2]);
    glUseProgram(shader);
    glBindAttribLocation(shader, 0, "vertex");
    glBindAttribLocation(shader, 1, "texCoord");
    glLinkProgram(shader);

    GLuint gQuad;
    glGenBuffers(1, &gQuad);
    glBindBuffer(GL_ARRAY_BUFFER, gQuad);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(tex_quad_vert), quad_mesh, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          false,
                          sizeof(tex_quad_vert),
                          ((tex_quad_vert*)nullptr)->location);

    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          false,
                          sizeof(tex_quad_vert),
                          ((tex_quad_vert*)nullptr)->uv);

    while(1)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                exit(0);
                break;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        glBindBuffer(GL_ARRAY_BUFFER, gQuad);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        SDL_GL_SwapWindow(window);
        SDL_Delay(50);
    }
    
    return 0;
}
