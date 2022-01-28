#ifndef ENGINEPROTOTYPE_ROTATION
#define ENGINEPROTOTYPE_ROTATION

#include <glm/glm.hpp>

class Transform {
    public:
        glm::vec3 position;
        glm::vec3 rotationDirection; 
        float angle;
        glm::vec3 scale;

        Transform();
        Transform(glm::vec3 position);
        Transform(glm::vec3 position, float angle, glm::vec3 rotationDirection);
        Transform(glm::vec3 position, float angle, glm::vec3 rotationDirection, glm::vec3 scale);

        glm::mat4& getModel();

    private: 
        glm::mat4 model;
};

#endif