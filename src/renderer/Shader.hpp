#pragma once

#include "renderer/OpenGL.hpp"

// Group shader programs and their uniform locations together
enum UniformId
{
    ModelViewProjectionMatrix,
    TextureMatrix,
    VertexColor,
    NUM_UNIFORMS
};

struct ShaderProgram
{
    GLuint id;
    GLuint vertShader;
    GLuint fragShader;
    GLuint geomShader;

    GLint uniforms[NUM_UNIFORMS];

    ShaderProgram() : id(0), vertShader(0), fragShader(0), geomShader(0)
    {
        for (int i = 0; i < NUM_UNIFORMS; i++)
        {
            uniforms[i] = -1;
        }
    }
};
