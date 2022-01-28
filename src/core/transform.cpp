#include "transform.h"
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(glm::vec3 position, float angle, glm::vec3 rotationDirection, glm::vec3 scale) {

    this->position = position;
    this->angle = angle; 
    this->rotationDirection = rotationDirection; 
    this->scale = scale;

};
Transform::Transform(glm::vec3 position, float angle, glm::vec3 rotationDirection) {
    Transform(position, angle, rotationDirection, glm::vec3(1.0f, 1.0f, 1.0f));
};
Transform::Transform(glm::vec3 position) {
    float angle = 0.0f;
    Transform(position, angle, glm::vec3(0.0f, 0.0f, 0.0f));
};
Transform::Transform() {
    Transform(glm::vec3(0.0f, 0.0f, 0.0f));
};

glm::mat4& Transform::getModel() {
    model = glm::mat4(1.0f);
    
    model = glm::rotate(model, angle, rotationDirection);
    model = glm::translate(model, position); 

    return model;
};