#ifndef ENGINEPROTOTYPE_WINDOW
#define ENGINEPROTOTYPE_WINDOW

#include <SDL2/SDL.h>

class Window {
    public:
        SDL_Window* sdlWindow;
        SDL_Event event;
        bool running = true; 
        bool framebufferResized = false;
        Window(); 
        ~Window();
};
#endif