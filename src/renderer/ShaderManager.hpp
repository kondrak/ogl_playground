#pragma once

#include "renderer/OpenGL.hpp"
#include "renderer/Shader.hpp"
#include <string>

class ShaderManager
{
public:
    // specific shaders
    enum ShaderName
    {
        BlankShader,
        BasicShader,
        FontShader,
        NUM_SHADERS
    };

    static ShaderManager* GetInstance();

    void LoadShaders();
    void DestroyShaders();

    const ShaderProgram& GetShaderProgram( ShaderName type ) const { return m_shaderProgram[type]; }
    const ShaderProgram& GetActiveShader() const { return m_shaderProgram[m_activeShader]; }
    const ShaderProgram& UseShaderProgram( ShaderName type );
private:
    ShaderManager() : m_activeShader( NUM_SHADERS )
    {
    }

    ~ShaderManager();

    std::string ReadShaderFromFile( const char *filename );
    void CompileShader( GLuint *newShader, GLenum shaderType, const char *shaderSrc );
    void LoadShader( ShaderName shaderName, const char* vshFilename, const char *fshFilename, const char *gshFilename = "" );
    bool LinkShader( GLuint* const pProgramObject, const GLuint VertexShader, const GLuint FragmentShader, const GLuint GeometryShader );

    ShaderName m_activeShader;
    ShaderProgram m_shaderProgram[NUM_SHADERS];
};
