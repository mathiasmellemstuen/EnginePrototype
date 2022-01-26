#include "keyboardInput.h"
#include "../utility/debug.h"
#include "keyData.h"
#include <vector>
#include <cctype>

void KeyboardInput::frameUpdate() {

    for(int i = 0; i < KeyboardInput::keys.size(); i++) {
        if(KeyboardInput::keys[i].up == true) {
            
            // Clearing every event that should only stay there for one frame, clearing those from the last frame
            KeyboardInput::keys.erase(KeyboardInput::keys.begin() + i); 
        }
        if(KeyboardInput::keys[i].down) {

            KeyboardInput::keys[i].down = false; 
            KeyboardInput::keys[i].pressed = true;
        }
    }
};


void KeyboardInput::update(SDL_Event& event) {

    if(event.type == SDL_KEYDOWN) {
        char key = *SDL_GetKeyName(event.key.keysym.sym);

        for(int i = 0; i < KeyboardInput::keys.size(); i++) {
            if(KeyboardInput::keys[i].key == key) {
                if(KeyboardInput::keys[i].pressed) 
                    return;
            }
        }

        KeyboardInput::keys.push_back({key, true, false, false});

    } else if(event.type == SDL_KEYUP) {
        char key = *SDL_GetKeyName(event.key.keysym.sym);

        for(int i = 0; i < KeyboardInput::keys.size(); i++) {
            if(KeyboardInput::keys[i].key == key) {

                KeyboardInput::keys[i].down = false; 
                KeyboardInput::keys[i].pressed = false; 
                KeyboardInput::keys[i].up = true; 
            }
        }
    }
};

bool KeyboardInput::keyDown(const char& key) {

    for(int i = 0; i < KeyboardInput::keys.size(); i++) {
        if(std::toupper(KeyboardInput::keys[i].key) == std::toupper(key)) {
            return KeyboardInput::keys[i].down; 
        }
    }
    return false;
};

bool KeyboardInput::keyUp(const char& key) {

    for(int i = 0; i < KeyboardInput::keys.size(); i++) {
        if(std::toupper(KeyboardInput::keys[i].key) == key) {
            return KeyboardInput::keys[i].up; 
        }
    }
    return false;
};

bool KeyboardInput::keyPressed(const char& key) {
    
    for(int i = 0; i < KeyboardInput::keys.size(); i++) {
        if(std::toupper(KeyboardInput::keys[i].key) == std::toupper(key)) {
            return KeyboardInput::keys[i].pressed; 
        }
    }
    return false;
};