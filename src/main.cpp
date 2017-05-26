#include "Application.hpp"
#include "DebugOverlay.hpp"
#include "InputHandlers.hpp"
#include "renderer/Font.hpp"
#include "renderer/RenderContext.hpp"


// keep the render context and application object global
RenderContext g_renderContext;
Application   g_application;
DebugOverlay *g_debugOverlay;

int main(int argc, char **argv)
{
    // initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0 ) 
    {
        return 1;
    }

    g_renderContext.Init("OpenGL Playground", 100, 100, 1024, 768);

    SDL_ShowCursor( SDL_DISABLE );

    if (glewInit() != GLEW_OK)
    {
        return 1;
   }

    g_application.OnStart( argc, argv );
    g_debugOverlay = new DebugOverlay();

    Uint32 last = SDL_GetTicks();

    while( g_application.Running() )
    {
        processEvents();

        Uint32 now = SDL_GetTicks();
        float dt = float(now - last) / 1000.f;

        g_debugOverlay->OnUpdate(dt);
        g_application.OnUpdate(dt);
        g_application.OnRender();

        g_debugOverlay->OnRender();

        SDL_GL_SwapWindow( g_renderContext.window );

        last = now;
    }

    delete g_debugOverlay;

    g_application.OnTerminate();

    g_renderContext.Destroy();
    SDL_Quit();

    return 0;
}
