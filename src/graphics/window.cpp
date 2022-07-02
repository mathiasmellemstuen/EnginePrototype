#include "window.h"

#include <SDL2/SDL.h>
#include "../utility/logging.h"
#include "SDL2/SDL_video.h"
#include <string>
#include "../utility/properties.h"

Window::Window() {

    logger(INFO, "Intializing SDL2"); 
    
    int code = SDL_Init(SDL_INIT_EVERYTHING);
    if(code < 0) {

        logger(ERROR, "Error while initializing SDL2!"); 
        logger(ERROR, "Error code " + std::to_string(code)); 
        std::string message = SDL_GetError();
        logger(ERROR, "Error message: " + message);
    }
    logger(SUCCESS, "SDL2 initiated!");
    logger(INFO, "Creating SDL2 window context"); 

    std::string title = PROPERTIES["windows"]["game"]["title"];

    int width = PROPERTIES["windows"]["game"]["resolution"]["width"];
    int height = PROPERTIES["windows"]["game"]["resolution"]["height"];
    sdlWindow = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS);

    int xPos = PROPERTIES["windows"]["game"]["position"]["x"]; 
    int yPos = PROPERTIES["windows"]["game"]["position"]["y"]; 
    SDL_SetWindowPosition(sdlWindow, xPos, yPos);

    logger(SUCCESS, "Successfully created SDL window context!"); 

};

void Window::clean() {
    logger(INFO, "Destroying SDL window");
    SDL_DestroyWindow(sdlWindow);
    sdlWindow = nullptr; 
    logger(SUCCESS, "Successfully destroyed SDL window!"); 
};
