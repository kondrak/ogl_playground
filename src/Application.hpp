#pragma once

#include <map>
#include "InputHandlers.hpp"


/*
 * main application
 */


class Application
{
public:
    void OnStart( int argc, char **argv );
    void OnRender();
    void OnUpdate( float dt );

    inline bool Running() const { return m_running; }
    inline void Terminate() { m_running = false; }

    void OnTerminate();
    bool KeyPressed( KeyCode key );
    void OnKeyPress( KeyCode key );
    void OnKeyRelease( KeyCode key );
    void OnMouseMove( int x, int y );
    void OnMouseDown( int button, int x, int y );
    void OnMouseUp( int button, int x, int y );
    void OnMouseWheel( int x, int y );
    void OnWindowSizeChanged( int width, int height );
private:
    inline void SetKeyPressed( KeyCode key, bool pressed ) { m_keyStates[key] = pressed; }

    bool m_running = true;
    std::map< KeyCode, bool > m_keyStates;
};
