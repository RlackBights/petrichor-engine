#ifndef PTC_CAMERA_HPP
#define PTC_CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ptc_component.hpp"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera : public Component
{
private:
    void updateCameraVectors();
public:
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec4 backgroundColor;
    static glm::vec3 WorldUp;

    static Camera* main;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    bool isBoosting;
    bool perspective;

    ~Camera();
    Camera(bool _perspective = true, bool isMain = false);
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix(int screenWidth, int screenHeight);
    void MoveCamera(Camera_Movement direction, float deltaTime);
    void FixedUpdate() override;
    void ProcessMouseScroll(float yoffset);
};

#endif