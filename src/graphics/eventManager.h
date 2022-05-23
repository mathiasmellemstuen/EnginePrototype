#ifndef ENGINEPROTOTYPE_EVENTMANAGER
#define ENGINEPROTOTYPE_EVENTMANAGER

#include <SDL2/SDL.h>
#include "window.h"

class EventManager {
    public:
        SDL_Event event;
        void update(Window& window);
};

#endif
