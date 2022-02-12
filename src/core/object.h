#ifndef ENGINEPROTOTYPE_OBJECT 
#define ENGINEPROTOTYPE_OBJECT

#include "../graphics/renderObject.h"
#include "transform.h"

#include <glm/glm.hpp>
#include <string>
#include <functional>
#include <vector>
#include "component.h"

class Object {
    public:
        static int NEXT_ID; 
        static std::vector<Object*> objects;

        int id;
        std::string name;
        Transform transform;
        RenderObject* renderObject;
        std::function<void(float& deltaTime)> update;
        std::function<void()> debug;
        std::vector<Component*> components;
        Object(const std::string& name, RenderObject* renderObject);
        virtual void addComponent(Component* component);
};

#endif