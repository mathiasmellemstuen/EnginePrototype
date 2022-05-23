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
        static bool keyDown(const std::string& key);
        static bool keyUp(const std::string& key);
        static bool keyPressed(const std::string& key);
        static bool keyHold(const std::string& key, const float& seconds);
};

#endif