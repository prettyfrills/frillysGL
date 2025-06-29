#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 3.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    right = glm::normalize(glm::cross(globalUp, forward));
    up = glm::cross(forward, right);
    pitch = 0.0f;
    yaw = -90.0f;
}

void Camera::Move(glm::vec3 direction)
{
    right = glm::normalize(glm::cross(globalUp, forward));
    glm::vec3 dir = forward * direction.z + right * direction.x + up * direction.y;
    position += dir * moveSpeed;
}

void Camera::Rotate(float x, float y)
{
    yaw += x;
    pitch -= y;

    if(pitch > 89.0f)
        pitch = 89.0;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction{};
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(direction);
}

glm::mat4 Camera::GetLookAt()
{
    return glm::lookAt(position, position + forward, globalUp);
}
