#ifndef ENGINEPROTOTYPE_EVENTMANAGER
#define ENGINEPROTOTYPE_EVENTMANAGER

#include <SDL2/SDL.h>

class EventManager {
    public:
        SDL_Event event;
        void update(SDL_Window& window);
};

#endif
