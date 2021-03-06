#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../utility/debug.h"
#include "../utility/logging.h"
#include "../core/object.h"
#include "../input/keyboardInput.h"
#include "../input/mouse.h"

void Camera::updateMatrices() {
    view = glm::lookAt(position, position + front, up);
    projection = glm::perspective(glm::radians(fov), aspectRatio, nearValue, farValue);
    projection[1][1] *= -1;
}

 Camera::Camera(const glm::vec3 &position, const float &fov, const float &aspectRatio, const float &nearValue, const float &farValue) {

    mainCamera = this;

    this->position = position;
    this->up = glm::vec3(0.0f, 0.0f, 1.0f);
    this->front = glm::vec3(1.0f, 0.0f, 0.0f);
    this->nearValue = nearValue;
    this->farValue = farValue;
    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->freeFlightSpeed = 10.0f;
    this->freeFlightSensitivity = 30.0f;
    this->mode = FREELOOK;

    updateMatrices();

}

glm::vec3 Camera::left() const {
    return glm::normalize(glm::cross(front, up));
}

void Camera::update(float& deltaTime) {

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

        // Keeping the angle between 360 and -360 degrees
        if(freeLookOrientation.x > 360 || freeLookOrientation.x < -360)
            freeLookOrientation.x = (int)freeLookOrientation.x % 360;

        float yaw = glm::radians(-freeLookOrientation.y);
        float pitch = glm::radians(-freeLookOrientation.x);

        front.x = glm::cos(yaw) * glm::cos(pitch);
        front.y = glm::sin(pitch);
        front.z = glm::sin(yaw);
        front = glm::normalize(front);

        updateMatrices();

}

void Camera::debug() {
    std::string title = std::string(Debug::selectedObject->name + " #" + std::to_string(Debug::selectedObject->id)).c_str();

    // Position
    float  arr[3] = {this->position.x, this->position.y, this->position.z};
    ImGui::InputFloat3("Position", arr);
    this->position = {arr[0], arr[1], arr[2]};

    // Up
    arr[0] = this->up.x;
    arr[1] = this->up.y;
    arr[2] = this->up.z;
    ImGui::InputFloat3("Up", arr);
    this->up = {arr[0], arr[1], arr[2]};

    // Front
    arr[0] = this->front.x;
    arr[1] = this->front.y;
    arr[2] = this->front.z;
    ImGui::InputFloat3("Front", arr);
    this->front = {arr[0], arr[1], arr[2]};

    // Freelook rotation
    arr[0] = this->freeLookOrientation.x;
    arr[1] = this->freeLookOrientation.y;
    ImGui::InputFloat2("Rotation (euler)", arr);
    this->freeLookOrientation = {arr[0], arr[1]};

    ImGui::InputFloat("Flight speed (units/second)", &freeFlightSpeed);
    ImGui::InputFloat("Free flight mouse sensitivity (degrees/second)", &freeFlightSensitivity);

    updateMatrices();
}