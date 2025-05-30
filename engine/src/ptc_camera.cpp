#include "ptc_input.h"
#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/trigonometric.hpp>
#include "ptc_camera.h"

Camera::Camera(bool _perspective, bool _main) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.6f), Zoom(100.0f)
{
    if (_perspective || main == nullptr) main = this;
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Front = glm::vec3(0.0f);
    Up = glm::vec3(0.0f);
    Right = glm::vec3(0.0f);
    backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    isBoosting = false;
    perspective = _perspective;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(parentObject->transform.position, parentObject->transform.position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix(int screenWidth, int screenHeight)
{
    if (perspective) {
        return glm::perspective(glm::radians(Zoom), (float)screenWidth / screenHeight, 0.1f, 100.0f);
    } else {
        float top = glm::tan(glm::radians(Zoom) / 2.0f);
        float right = top * (float)screenWidth / screenHeight;
        return glm::ortho(-right, right, -top, top, 0.1f, 100.0f);
    }
}

void Camera::MoveCamera(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    glm::vec3 moveVec = glm::vec3(0.0f);
    if (direction == FORWARD)
        moveVec += Front * velocity;
    if (direction == BACKWARD)
        moveVec -= Front * velocity;
    if (direction == LEFT)
        moveVec -= Right * velocity;
    if (direction == RIGHT)
        moveVec += Right * velocity;
    if (direction == UP)
        moveVec += Up * velocity;
    if (direction == DOWN)
        moveVec -= Up * velocity;

    parentObject->transform.position += moveVec * (isBoosting ? 2.0f : 1.0f);
}

void Camera::FixedUpdate()
{
    float yaw   = glm::radians(-Input::mouseXrel * MouseSensitivity);
    float pitch = glm::radians(Input::mouseYrel * MouseSensitivity);
    float zoom  = Input::mouseScroll;

    // Get the forward direction from the current quaternion
    glm::vec3 forward = parentObject->transform.rotation * glm::vec3(0, 0, -1);

    // Extract current pitch using arcsin (safe method)
    float currentPitch = glm::degrees(glm::asin(forward.y)); // Get pitch from forward vector

    // Clamp the new pitch value
    float maxPitch = 89.0f; // Prevent flipping
    float newPitch = glm::clamp(currentPitch + glm::degrees(pitch), -maxPitch, maxPitch);

    // Compute new pitch rotation
    glm::quat pitchRotation = glm::angleAxis(glm::radians(newPitch - currentPitch), glm::vec3(1, 0, 0));

    // Compute yaw rotation as normal
    glm::quat yawRotation = glm::angleAxis(yaw, glm::vec3(0, 1, 0));

    // Apply the yaw first, then the clamped pitch
    parentObject->transform.rotation = yawRotation * parentObject->transform.rotation;
    parentObject->transform.rotation = parentObject->transform.rotation * pitchRotation;

    // Normalize to avoid drift
    parentObject->transform.rotation = glm::normalize(parentObject->transform.rotation);

    if (zoom != 0.0f) ProcessMouseScroll(zoom);

    updateCameraVectors();
}



void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom = glm::clamp<float>(Zoom - (float)yoffset * 2.0f, 20.0f, 100.0f);
}

void Camera::updateCameraVectors()
{
    glm::quat rotation = parentObject->transform.rotation;

    glm::vec3 front = rotation * glm::vec3(0.0f, 0.0f, -1.0f);

    glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);

    Front = glm::normalize(front);
    Right = glm::normalize(right);
    Up = glm::normalize(up);
}

Camera* Camera::main;
glm::vec3 Camera::WorldUp;