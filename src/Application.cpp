#include <SDL.h>
#include "Application.hpp"
#include "renderer/CameraDirector.hpp"
#include "renderer/RenderContext.hpp"
#include "renderer/ShaderManager.hpp"
#include "renderer/Texture.hpp"
#include "renderer/TextureManager.hpp"
#include "DebugOverlay.hpp"


extern RenderContext g_renderContext;
extern DebugOverlay *g_debugOverlay;
CameraDirector       g_cameraDirector;

// test render data
GLuint g_vertexBufferTriangle;
GLuint g_vertexBufferQuad;
GLuint g_colorBuffer;
GLuint g_texCoordBuffer;
GLuint g_vertexArrayTriangle;
GLuint g_vertexArrayQuad;

Texture *blockImage;

void Application::OnStart( int argc, char **argv )
{
    glViewport( 0, 0, g_renderContext.width, g_renderContext.height );

    g_cameraDirector.AddCamera( Math::Vector3f( 0.0f, 0.0f, 1.0f ),
                                Math::Vector3f( 0.f, 1.f, 0.f ),
                                Math::Vector3f( 1.f, 0.f, 0.f ),
                                Math::Vector3f( 0.f, 0.f, -1.f ) );

    g_cameraDirector.GetActiveCamera()->SetMode( Camera::CAM_FPS );

    glEnable( GL_DEPTH_TEST );

    ShaderManager::GetInstance()->LoadShaders();

    const GLfloat triangleVertexBuffer[] = { -2.5f, -1.0f, -2.5f,
                                             -1.5f, 1.0f, -2.5f,
                                             -0.5f, -1.0f, -2.5f, };
    // x shaped - because triangle strips
    const GLfloat quadVertexBuffer[] = {  0.0f, 1.0f, -2.5f,
                                          2.0f, 1.0f, -2.5f,
                                          0.0f, -1.0f, -2.5f,
                                          2.0f, -1.0f, -2.5f };

    const GLfloat colorBufferData[] = { 1.0f, 0.0f, 0.0f, 1.0f,
                                        0.0f, 1.0f, 0.0f, 1.0f,
                                        0.0f, 0.0f, 1.0f, 1.0f,
                                        1.0f, 0.0f, 1.0f, 1.0f };

    const GLfloat texCoordBufferData[] = { 0.0f, 0.0f,
                                           1.0f, 0.0f,
                                           0.0f, 1.0f,
                                           1.0f, 1.0f };

    blockImage = TextureManager::GetInstance()->LoadTexture( "textures/block_blue.png" );

    // create triangle VAO & VBO
    glGenVertexArrays( 1, &g_vertexArrayTriangle );
    glBindVertexArray( g_vertexArrayTriangle );

    glGenBuffers( 1, &g_vertexBufferTriangle );
    glBindBuffer( GL_ARRAY_BUFFER, g_vertexBufferTriangle );
    glBufferData( GL_ARRAY_BUFFER, sizeof( triangleVertexBuffer ), triangleVertexBuffer, GL_STATIC_DRAW );

    // create quad VAO & VBO
    glGenVertexArrays( 1, &g_vertexArrayQuad );
    glBindVertexArray( g_vertexArrayQuad );

    glGenBuffers( 1, &g_vertexBufferQuad );
    glBindBuffer( GL_ARRAY_BUFFER, g_vertexBufferQuad );
    glBufferData( GL_ARRAY_BUFFER, sizeof( quadVertexBuffer ), quadVertexBuffer, GL_STATIC_DRAW );

    // create color & texture buffers
    glGenBuffers( 1, &g_colorBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, g_colorBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( colorBufferData ), colorBufferData, GL_STATIC_DRAW );

    glGenBuffers( 1, &g_texCoordBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, g_texCoordBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( texCoordBufferData ), texCoordBufferData, GL_STATIC_DRAW );
}

void Application::OnRender()
{
    // clear screen
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_ACCUM_BUFFER_BIT );

    g_cameraDirector.GetActiveCamera()->OnRender();

    const ShaderProgram &shader = ShaderManager::GetInstance()->UseShaderProgram( ShaderManager::BasicShader );

    GLuint vertexPosition_modelspaceID = glGetAttribLocation( shader.id, "inVertex" );
    GLuint vertexColorAttr = glGetAttribLocation( shader.id, "inVertexColor" );
    GLuint texCoordAttr = glGetAttribLocation( shader.id, "inTexCoord" );

    TextureManager::GetInstance()->BindTexture( blockImage );

    // triangle
    glBindVertexArray( g_vertexArrayTriangle );
    glEnableVertexAttribArray( vertexPosition_modelspaceID );
    glEnableVertexAttribArray( vertexColorAttr );
    glEnableVertexAttribArray( texCoordAttr );

    glBindBuffer( GL_ARRAY_BUFFER, g_vertexBufferTriangle );
    glVertexAttribPointer( vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    glBindBuffer( GL_ARRAY_BUFFER, g_colorBuffer );
    glVertexAttribPointer( vertexColorAttr, 4, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    glBindBuffer( GL_ARRAY_BUFFER, g_texCoordBuffer );
    glVertexAttribPointer( texCoordAttr, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    // textured quad
    glBindVertexArray( g_vertexArrayQuad );
    glEnableVertexAttribArray( vertexPosition_modelspaceID );
    glEnableVertexAttribArray( vertexColorAttr );
    glEnableVertexAttribArray( texCoordAttr );

    glBindBuffer( GL_ARRAY_BUFFER, g_vertexBufferQuad );
    glVertexAttribPointer( vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    glBindBuffer( GL_ARRAY_BUFFER, g_colorBuffer );
    glVertexAttribPointer( vertexColorAttr, 4, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    glBindBuffer( GL_ARRAY_BUFFER, g_texCoordBuffer );
    glVertexAttribPointer( texCoordAttr, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    Math::Matrix4f MVP = g_cameraDirector.GetActiveCamera()->ViewMatrix() * g_cameraDirector.GetActiveCamera()->ProjectionMatrix();

    // update matrix
    glUniformMatrix4fv( shader.uniforms[ModelViewProjectionMatrix], 1, GL_FALSE, &MVP[0] );

    // draw the triangle!
    glBindVertexArray( g_vertexArrayTriangle );
    glDrawArrays( GL_TRIANGLES, 0, 3 ); // 3 indices starting at 0 -> 1 triangle

    // draw the quad!
    glBindVertexArray( g_vertexArrayQuad );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 ); // 4 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray( vertexPosition_modelspaceID );
    glDisableVertexAttribArray( vertexColorAttr );
    glDisableVertexAttribArray( texCoordAttr );
}


void Application::OnUpdate( float dt )
{
    if( KeyPressed( KEY_A ) )
        g_cameraDirector.GetActiveCamera()->Strafe( -8.f * dt );

    if( KeyPressed( KEY_D ) )
        g_cameraDirector.GetActiveCamera()->Strafe( 8.f * dt );

    if( KeyPressed( KEY_W ) )
        g_cameraDirector.GetActiveCamera()->MoveForward( -8.f * dt );

    if( KeyPressed( KEY_S ) )
        g_cameraDirector.GetActiveCamera()->MoveForward( 8.f * dt );

    // do the barrel roll!
    if( KeyPressed( KEY_Q ) )
        g_cameraDirector.GetActiveCamera()->rotateZ( 2.f * dt );

    if( KeyPressed( KEY_E ) )
        g_cameraDirector.GetActiveCamera()->rotateZ( -2.f * dt );

    // move straight up/down
    if( KeyPressed( KEY_R ) )
        g_cameraDirector.GetActiveCamera()->MoveUpward( 8.f * dt );

    if( KeyPressed( KEY_F ) )
        g_cameraDirector.GetActiveCamera()->MoveUpward( -8.f * dt );
}


void Application::OnTerminate()
{
    // cleanup
    glDeleteBuffers( 1, &g_vertexBufferTriangle );
    glDeleteBuffers( 1, &g_colorBuffer );
    glDeleteBuffers( 1, &g_vertexBufferQuad );
    glDeleteBuffers( 1, &g_texCoordBuffer );
    glDeleteVertexArrays( 1, &g_vertexArrayTriangle );
    glDeleteVertexArrays( 1, &g_vertexArrayQuad );
}


bool Application::KeyPressed( KeyCode key )
{
    // to be 100% no undefined state exists
    if( m_keyStates.find( key ) == m_keyStates.end() )
        m_keyStates[key] = false;

    return m_keyStates[key];
}


void Application::OnKeyPress( KeyCode key )
{
    SetKeyPressed( key, true );

    switch( key )
    {
    case KEY_F1:
    {
        GLint pMode[2];
        glGetIntegerv( GL_POLYGON_MODE, pMode );

        if( pMode[0] == GL_LINE )
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        else
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    break;
    case KEY_F12:
        SDL_GL_SetSwapInterval( SDL_GL_GetSwapInterval() ^ 1 );
        break;
    case KEY_ESC:
        Terminate();
        break;
    default:
        break;
    }

    g_debugOverlay->OnKeyPress( key );
}


void Application::OnKeyRelease( KeyCode key )
{
    SetKeyPressed( key, false );
}


void Application::OnMouseMove( int x, int y )
{
    g_cameraDirector.GetActiveCamera()->OnMouseMove( x, y );
}

void Application::OnMouseDown( int button, int x, int y )
{

}

void Application::OnMouseUp( int button, int x, int y )
{

}

void Application::OnMouseWheel( int x, int y )
{

}

void Application::OnWindowSizeChanged( int width, int height )
{
    g_renderContext.width = width;
    g_renderContext.height = height;
    g_renderContext.halfWidth = width >> 1;
    g_renderContext.halfHeight = height >> 1;
    g_renderContext.scrRatio = (float)width / (float)height;

    glViewport( 0, 0, width, height );

    g_cameraDirector.GetActiveCamera()->UpdateProjectionMatrix();
}
