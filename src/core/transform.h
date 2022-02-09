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
        Transform(const glm::vec3& position);
        Transform(const glm::vec3& position, const float& angle, const glm::vec3& rotationDirection);
        Transform(const glm::vec3& position, const float& angle, const glm::vec3& rotationDirection, const glm::vec3& scale);

        glm::mat4& getModel();

    private: 
        glm::mat4 model;
};

#endif