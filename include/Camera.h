#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>

class Camera
{
    private:
    const glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float moveSpeed = 2.0f;

    // TODO: Make readonly.
    public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, -3.0f);
    glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right = glm::normalize(glm::cross(globalUp, forward));
    glm::vec3 up = glm::cross(forward, right);
    float pitch = 0.0f;
    float yaw = -90.0f;

    Camera();
    void Move(glm::vec3 direction);
    void Rotate(float x, float y);
    glm::mat4 GetLookAt();
};

#endif