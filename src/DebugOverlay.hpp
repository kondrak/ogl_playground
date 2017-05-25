#pragma once

#include "renderer/Font.hpp"
#include "InputHandlers.hpp"
#include <sstream>

class FPSCounter
{
public:
    FPSCounter() : m_numFrames( 0 ), m_numFramesToDraw( 1 ), m_lastTime( 1000 )
    {
        m_font = new Font( "textures/font.png" );
    }

    ~FPSCounter()
    {
        delete m_font;
    }

    void OnFrameStart()
    {
        m_numFrames++;
    }

    void OnRender()
    {
        std::stringstream sstream, sstream2;
        sstream << m_numFramesToDraw << " FPS";
        sstream2 << 1000.f / m_numFramesToDraw << " ms";
        m_font->drawText( sstream.str().c_str(), -1.0f, 1.0f );
        m_font->drawText( sstream2.str().c_str(), -1.0f, 0.975f );

        if( SDL_GetTicks() - m_lastTime >= 1000 )
        {
            m_numFramesToDraw = m_numFrames;
            m_numFrames = 0;
            m_lastTime = SDL_GetTicks();
        }
    }

private:
    Font *m_font;
    int m_numFrames;
    int m_numFramesToDraw;
    int m_lastTime;
};


class DebugOverlay
{
public:
    enum DebugFlag
    {
        DEBUG_NONE = 0,
        DEBUG_SHOW_FPS = 1 << 0
    };

    DebugOverlay() : m_debugFlags( DEBUG_SHOW_FPS )
    {
    }

    void OnUpdate( float dt );
    void OnRender();
    void OnKeyPress( KeyCode key );
    bool DebugFlagSet( DebugFlag df ) { return ( m_debugFlags & df ) != 0; }
private:
    FPSCounter m_fpsCounter;
    int  m_debugFlags;
};
