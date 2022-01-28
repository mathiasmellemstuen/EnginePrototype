#ifndef ENGINEPROTOTYPE_OBJECT 
#define ENGINEPROTOTYPE_OBJECT

#include "../graphics/renderObject.h"
#include "transform.h"

#include <glm/glm.hpp>
#include <string>
#include <functional>

class Object {
    public:
        static int NEXT_ID; 
        static std::vector<Object> objects;
        int id;

        std::string& name;
        Transform& transform;
        RenderObject& renderObject;
        std::function<void(float& deltaTime)> update;
        Object(std::string& name, Transform& transform, RenderObject& renderObject);
        void setUpdate(std::function<void(float& deltaTime)>& update);

};

#endif