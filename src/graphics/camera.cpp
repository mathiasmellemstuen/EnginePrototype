#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../utility/debug.h"

#include "../core/object.h"
#include "../input/keyboardInput.h"
#include "../input/mouse.h"

void Camera::updateMatrices() {
    view = glm::lookAt(position, position + front, up);
    projection = glm::perspective(fov, aspectRatio, near, far);
    projection[1][1] *= -1;
}

 Camera::Camera(const glm::vec3 &position, const float &fov, const float &aspectRatio, const float &near, const float &far) : view(1), projection(1), position(0), up(0), freeLookOrientation(0),  Object("Camera", nullptr) {

    mainCamera = this;

    this->position = position;

    this->up = glm::vec3(0.0f, 0.0f, 1.0f);
    this->front = glm::vec3(1.0f, 0.0f, 0.0f);

    this->near = near;
    this->far = far;
    this->fov = fov;
    this->aspectRatio = aspectRatio;

    this->freeFlightSpeed = 0.02f;
    this->freeFlightSensitivity = 0.05f;

    this->mode = FREELOOK;
    updateMatrices();

    update = [&](float& deltaTime) {

        // Can click on F1 to toggle freelook mode.
        if(KeyboardInput::keyDown("F1")) {
            if(Mouse::isMouseRelative()) {
                mode = STATIC;
                Mouse::disableRelativeMouse();
            } else {
                mode = FREELOOK;
                Mouse::enableRelativeMouse();
            }
        }

        if(mode == STATIC)
            return;

        if(KeyboardInput::keyPressed("w")) {
            this->position += this->freeFlightSpeed * deltaTime * front;
        }
        if(KeyboardInput::keyPressed("s")) {
            this->position -= this->freeFlightSpeed * deltaTime * front;
        }
        if(KeyboardInput::keyPressed("a")) {
            this->position -= this->freeFlightSpeed * deltaTime * left();
        }
        if(KeyboardInput::keyPressed("d")) {
            this->position += this->freeFlightSpeed * deltaTime * left();
        }
        if(KeyboardInput::keyPressed("space")) {
            this->position += this->freeFlightSpeed * deltaTime * up;
        }
        if(KeyboardInput::keyPressed("left ctrl")) {
            this->position -= this->freeFlightSpeed * deltaTime * up;
        }
        freeLookOrientation.x += Mouse::mouseAcceleration.x * deltaTime * freeFlightSensitivity;
        freeLookOrientation.y += Mouse::mouseAcceleration.y * deltaTime * freeFlightSensitivity;

        if(freeLookOrientation.y > 89.0f) {
            freeLookOrientation.y = 89.0f;
        }
        if(freeLookOrientation.y < -89.0f) {
            freeLookOrientation.y = -89.0f;
        }
        float yaw = -freeLookOrientation.y;
        float pitch = -freeLookOrientation.x;
        glm::vec3 direction;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);

        updateMatrices();
    };

}

glm::vec3 Camera::left() const {
    return glm::normalize(glm::cross(front, up));
}
