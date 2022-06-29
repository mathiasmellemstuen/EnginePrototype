#include "object.h"

template<class T> T* Object::getComponent() {
    for(Component* component : components) {
        if(dynamic_cast<T*>(component) != 0) {
                return (T*)component;
            }
    }
    return nullptr;
}