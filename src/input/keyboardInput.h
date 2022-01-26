#ifndef ENGINEPROTOTYPE_KEYBOARD_INPUT
#define ENGINEPROTOTYPE_KEYBOARD_INPUT

#include <SDL2/SDL.h>
#include <vector>

#include "keyData.h"


// Potential bug with CTRL+J because of using \n for blank entries.
class KeyboardInput {
    public:
        inline static std::vector<KeyData> keys;
        static void update(SDL_Event& event);
        static void frameUpdate();
        static bool keyDown(const char& key); 
        static bool keyUp(const char& key);
        static bool keyPressed(const char& key);
};

#endif