#include "component.h"

#include <imgui/imgui.h>

int Component::NEXT_COMPONENT = 0; 

Component::Component() {

    id = NEXT_COMPONENT;
    NEXT_COMPONENT += 1;

    name = "Component";

    debug = [&](){
    };
};