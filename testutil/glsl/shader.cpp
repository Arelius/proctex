#include <SDL_opengl.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define GL_DECL(fn, dcl) extern dcl fn;
#include "glext_loader.inc"
#undef GL_DECL

#define log(svr, msg) OutputDebugStringA(msg)

const char* shaderHeader = "#version 420\n";

char* ReadShaderSourceFromFile(const char* filename)
{
    size_t headerLen = strlen(shaderHeader);

    FILE* file = fopen(filename, "r");

    if(!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    size_t len = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* ret = (char*)malloc(headerLen+len+1);
    memcpy(ret, shaderHeader, headerLen);
    size_t readc = fread(ret+headerLen, 1, len, file);
    // This seems to come out smaller on Win32.
    assert(readc <= len);
    ret[headerLen+readc] = '\0';

    fclose(file);

    return ret;
}

GLuint CreateShader(const char* filename, GLenum type)
{
    GLuint shader = glCreateShader(type);
    assert(shader);

    char* src[1];
    src[0] = ReadShaderSourceFromFile(filename);
    int len[1];
    len[0] = -1;

    glShaderSource(shader, 1, (const char**)src, len);
    free(src[0]);

    glCompileShader(shader);

    GLint link_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &link_status);

    if(link_status == GL_FALSE)
    {
        size_t buf_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, (GLint*)&buf_len);
        char* buffer = (char*)malloc(buf_len);
        size_t read_len = 0;
        glGetShaderInfoLog(shader, buf_len, (GLsizei*)&read_len, buffer);
        assert(buf_len == read_len + 1);
        log(LOG_ERROR, buffer);
        free(buffer);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint CreateShaderProgram(const char* vfilename, const char* pfilename)
{
    GLuint vshader, gshader, pshader, program;
    vshader = CreateShader(vfilename, GL_VERTEX_SHADER);
    if(!vshader) goto error;

    pshader = CreateShader(pfilename, GL_FRAGMENT_SHADER);
    if(!pshader) goto error_pshader;

    gshader = NULL;

    program = glCreateProgram();
    assert(program);

    glAttachShader(program, vshader);
    glAttachShader(program, pshader);
    if(gshader)
        glAttachShader(program, gshader);

    glLinkProgram(program);

    GLint link_status;

    glGetProgramiv(program, GL_LINK_STATUS, &link_status);

     if(link_status == GL_FALSE)
     {
         size_t buf_len = 0;
         glGetProgramiv(program, GL_INFO_LOG_LENGTH, (GLsizei*)&buf_len);
         if(buf_len)
         {
             char* buffer = (char*)malloc(buf_len);
             assert(buffer);
             size_t read_len = 0;
             glGetProgramInfoLog(program, buf_len, (GLsizei*)&read_len, buffer);
             assert(buf_len == read_len + 1);
             buffer[buf_len] = '\0';
             log(LOG_ERROR, buffer);
             free(buffer);
         }
         else
         {
             log(LOG_ERROR, "Unknown error linking program");
         }
         goto error_program;
     }

     glDeleteShader(pshader);
     glDeleteShader(vshader);
     if(gshader)
         glDeleteShader(gshader);

     return program;

error_program:
     glDetachShader(program, pshader);
     glDetachShader(program, vshader);
     if(gshader)
         glDetachShader(program, gshader);
     glDeleteProgram(program);
     if(gshader)
         glDeleteShader(gshader);
error_gshader:
     glDeleteShader(pshader);
error_pshader:
     glDeleteShader(vshader);
error:
     return 0;
}

void DestroyProgramAndAttachedShaders(GLuint program)
{
    glDeleteProgram(program);
}
