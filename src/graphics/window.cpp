#include "window.h"

#include <SDL2/SDL.h>
#include "../utility/properties.h"
#include "../utility/log.h"

Window::Window() {

    log(INFO, "Intializing SDL2"); 

    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        log(ERROR, "Error while initializing SDL2!"); 
    }
    log(SUCCESS, "SDL2 initiated!");
    log(INFO, "Creating SDL2 window context"); 

    sdlWindow = SDL_CreateWindow(properties.title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,properties.screenWidth,properties.screenHeight,SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
    log(SUCCESS, "Successfully created GLFW window context!"); 

};

Window::~Window() {
    log(INFO, "Destroying GLFW window");
    SDL_DestroyWindow(sdlWindow);
    sdlWindow = nullptr; 
    log(SUCCESS, "Successfully destroyed GLFW window!"); 
};