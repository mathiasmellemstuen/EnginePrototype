#include "keyboardInput.h"
#include "../utility/debug.h"
#include "keyData.h"
#include <vector>

void KeyboardInput::frameUpdate() {
    // Clearing every event that should only stay there for one frame, clearing those from the last frame
    for(int i = 0; i < KeyboardInput::keys.size(); i++) {
        if(KeyboardInput::keys[i].up == true) {
            
            KeyboardInput::keys.erase(KeyboardInput::keys.begin() + i); 
            // KeyboardInput::keys[i].key = '\n';
            // KeyboardInput::keys[i].pressed = false; 
            // KeyboardInput::keys[i].down = false; 
            // KeyboardInput::keys[i].up = false;
        }
    }

    // if(KeyboardInput::keys[0].key == '\n') {
    //     for(int i = 1; i < 4; i++) {
    //         KeyboardInput::keys[i - 1] = KeyboardInput::keys[i]; 
    //     }
    // }
}

void KeyboardInput::update(SDL_Event& event) {

    if(event.type == SDL_KEYDOWN) {
        char key = *SDL_GetKeyName(event.key.keysym.sym);

        for(int i = 0; i < KeyboardInput::keys.size(); i++) {
            if(KeyboardInput::keys[i].key == key) {
                
                if(KeyboardInput::keys[i].pressed) 
                    return;

                if(KeyboardInput::keys[i].down) {

                    KeyboardInput::keys[i].down = false; 
                    KeyboardInput::keys[i].pressed = true;
                    return; 
                }
            }
            //  else if(KeyboardInput::keys[i].key == '\n') {

            //         KeyboardInput::keys[i].key = key; 
            //         KeyboardInput::keys[i].down = true;
            //         return;
            // }
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