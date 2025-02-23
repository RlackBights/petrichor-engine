#ifndef PTC_CAMERA_H
#define PTC_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ptc_transform.h>
#include <ptc_component.h>

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
    static Camera* mainCamera;
public:
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    static glm::vec3 WorldUp;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    bool isBoosting;

    Camera(bool isMain = false);
    static Camera* getMainCamera();
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix(int screenWidth, int screenHeight);
    void MoveCamera(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true, float zoom = 0.0f);
    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};

#endif