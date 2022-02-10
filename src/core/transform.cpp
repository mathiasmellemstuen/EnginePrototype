#include "transform.h"
#include "../utility/debug.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Transform::Transform(const glm::vec3& position, const float& angle, const glm::vec3& rotationDirection, const glm::vec3& scale) {

    this->position = position;
    this->angle = angle; 
    this->rotationDirection = rotationDirection; 
    this->scale = scale;

};
Transform::Transform(const glm::vec3& position, const float& angle, const glm::vec3& rotationDirection) {
    this->position = position;
    this->angle = angle;
    this->rotationDirection = rotationDirection;
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
};
Transform::Transform(const glm::vec3& position) {
    this->position = position;
    this->angle = 0;
    this->rotationDirection = glm::vec3(0.0f, 0.0f, 1.0);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
};
Transform::Transform() {
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->angle = 0;
    this->rotationDirection = glm::vec3(0.0f, 0.0f, 1.0);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

glm::mat4& Transform::getModel() {

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotationDirection);
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

    model = translation * rotation;

    return model;
};