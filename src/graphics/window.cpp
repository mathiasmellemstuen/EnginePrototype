#include "window.h"

#include <SDL2/SDL.h>
#include "../utility/debug.h"
#include "SDL2/SDL_video.h"
#include <string>
#include "../utility/properties.h"

Window::Window() {

    Debug::log(INFO, "Intializing SDL2"); 
    
    int code = SDL_Init(SDL_INIT_EVERYTHING);
    if(code < 0) {

        Debug::log(ERROR, "Error while initializing SDL2!"); 
        Debug::log(ERROR, "Error code " + std::to_string(code)); 
        std::string message = SDL_GetError();
        Debug::log(ERROR, "Error message: " + message);
    }
    Debug::log(SUCCESS, "SDL2 initiated!");
    Debug::log(INFO, "Creating SDL2 window context"); 

    std::string title = (*properties)["windows"]["game"]["title"];

    int width = (*properties)["windows"]["game"]["resolution"]["width"];
    int height = (*properties)["windows"]["game"]["resolution"]["height"];
    sdlWindow = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS);

    int xPos = (*properties)["windows"]["game"]["position"]["x"]; 
    int yPos = (*properties)["windows"]["game"]["position"]["y"]; 
    SDL_SetWindowPosition(sdlWindow, xPos, yPos);

    Debug::log(SUCCESS, "Successfully created SDL window context!"); 

};

void Window::clean() {
    Debug::log(INFO, "Destroying SDL window");
    SDL_DestroyWindow(sdlWindow);
    sdlWindow = nullptr; 
    Debug::log(SUCCESS, "Successfully destroyed SDL window!"); 
};
