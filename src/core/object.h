#ifndef ENGINEPROTOTYPE_OBJECT 
#define ENGINEPROTOTYPE_OBJECT

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
        virtual void update(float& deltaTime);
        virtual void debug();
        virtual void addComponent(Component* component);
        std::vector<Component*> components;
        Object(const std::string& name);
        
        template<class T> T* getComponent();
};

#include "object.tpp"

#endif
