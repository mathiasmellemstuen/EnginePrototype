#ifndef ENGINEPROTOTYPE_KEY_DATA
#define ENGINEPROTOTYPE_KEY_DATA

struct KeyData {
    char key = '\n'; 
    bool down = false;
    bool pressed = false; 
    bool up = false; 
};

#endif