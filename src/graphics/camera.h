//
// Created by Mathias on 11.02.2022.
//

#ifndef ENGINEPROTOTYPE_CAMERA_H
#define ENGINEPROTOTYPE_CAMERA_H

#include <glm/glm.hpp>
#include "../core/component.h"

enum CameraMode {
    FREELOOK,
    STATIC
};


class Camera : public Component {
public:
    inline static Camera* mainCamera;

    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 front;
    glm::vec2 freeLookOrientation;

    float nearValue;
    float farValue;
    float fov;
    float aspectRatio;

    float freeFlightSpeed;
    float freeFlightSensitivity;

    CameraMode mode;

    void updateMatrices();
    virtual void debug();
    virtual void update(float& deltaTime);

    glm::vec3 left() const;

    Camera(const glm::vec3& position,const float& fov, const float& aspectRatio, const float& nearValue, const float& farValue);
};

#endif //ENGINEPROTOTYPE_CAMERA_H