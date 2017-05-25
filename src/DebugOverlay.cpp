#include "DebugOverlay.hpp"

void DebugOverlay::OnUpdate( float dt )
{
    if( m_debugFlags & DEBUG_SHOW_FPS )
        m_fpsCounter.OnFrameStart();
}

void DebugOverlay::OnRender()
{
    GLint pMode[2];
    glGetIntegerv( GL_POLYGON_MODE, pMode );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    if( m_debugFlags & DEBUG_SHOW_FPS )
        m_fpsCounter.OnRender();

    glPolygonMode( GL_FRONT_AND_BACK, pMode[0] );
}

void DebugOverlay::OnKeyPress( KeyCode key )
{
    switch( key )
    {
    case KEY_F11:
        m_debugFlags ^= DEBUG_SHOW_FPS;
        break;
    }
}
