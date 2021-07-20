#include "window.h"

#include <SDL2/SDL.h>
#include "../utility/log.h"
#include <string>

Window::Window() {

    log(INFO, "Intializing SDL2"); 

    int code = SDL_Init(SDL_INIT_EVERYTHING);
    if(code < 0) {

        log(ERROR, "Error while initializing SDL2!"); 
        log(ERROR, "Error code " + std::to_string(code)); 
        std::string message = SDL_GetError();
        log(ERROR, "Error message: " + message);
    }
    log(SUCCESS, "SDL2 initiated!");
    log(INFO, "Creating SDL2 window context"); 
    //TODO: Change values in createWindow to properties values instead of hardcoded values. 
    sdlWindow = SDL_CreateWindow("Vulkan",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600,SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    log(SUCCESS, "Successfully created SDL window context!"); 
};

Window::~Window() {
    log(INFO, "Destroying SDL window");
    SDL_DestroyWindow(sdlWindow);
    sdlWindow = nullptr; 
    log(SUCCESS, "Successfully destroyed SDL window!"); 
};