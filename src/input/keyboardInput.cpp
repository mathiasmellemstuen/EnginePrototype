#include "keyboardInput.h"
#include "../utility/logging.h"
#include <string>
#include <ctype.h>

std::string convertStringToUpper(const std::string& key) {

    std::string nKey = "";

    for(int i = 0; i < key.size(); i++) {
        nKey += std::toupper(key[i]);
    }

    return nKey;
}

void KeyboardInput::frameUpdate() {

    for(int i = 0; i < KeyboardInput::keys.size(); i++) {
        if(KeyboardInput::keys[i].up) {
            
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

        std::string key = convertStringToUpper(std::string(SDL_GetKeyName(event.key.keysym.sym)));

        for(int i = 0; i < KeyboardInput::keys.size(); i++) {
            if(KeyboardInput::keys[i].key == key) {
                if(KeyboardInput::keys[i].pressed)
                    return;
            }
        }


        KeyboardInput::keys.push_back({key, true, false, false});

    } else if(event.type == SDL_KEYUP) {
        std::string key = convertStringToUpper(std::string(SDL_GetKeyName(event.key.keysym.sym)));

        for(int i = 0; i < KeyboardInput::keys.size(); i++) {
            if(KeyboardInput::keys[i].key == key) {

                KeyboardInput::keys[i].down = false; 
                KeyboardInput::keys[i].pressed = false; 
                KeyboardInput::keys[i].up = true; 
            }
        }
    }
};


bool KeyboardInput::keyDown(const std::string& key) {

    std::string upperKey = convertStringToUpper(key);

    for(int i = 0; i < KeyboardInput::keys.size(); i++) {
        if(KeyboardInput::keys[i].key.c_str() == upperKey) {
            return KeyboardInput::keys[i].down; 
        }
    }
    return false;
};

bool KeyboardInput::keyUp(const std::string& key) {

    std::string upperKey = convertStringToUpper(key);

    for(int i = 0; i < KeyboardInput::keys.size(); i++) {
        if(KeyboardInput::keys[i].key == upperKey) {
            return KeyboardInput::keys[i].up; 
        }
    }
    return false;
};

bool KeyboardInput::keyPressed(const std::string& key) {

    std::string upperKey = convertStringToUpper(key);

    for(int i = 0; i < KeyboardInput::keys.size(); i++) {

        if(KeyboardInput::keys[i].key == upperKey) {
            return KeyboardInput::keys[i].pressed; 
        }
    }
    return false;
};