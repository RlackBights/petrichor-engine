#include <ptc_camera.h>

Camera::Camera(bool isMain) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.6f), Zoom(100.0f)
{
    if (isMain || mainCamera == nullptr) mainCamera = this;
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Front = glm::vec3(0.0f);
    Up = glm::vec3(0.0f);
    Right = glm::vec3(0.0f);
    isBoosting = false;
}

Camera* Camera::getMainCamera()
{
    return mainCamera;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(parentObject->transform.position, parentObject->transform.position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix(int screenWidth, int screenHeight)
{
    return glm::perspective(glm::radians(Zoom), (float)screenWidth / screenHeight, 0.1f, 100.0f);
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
        moveVec += WorldUp * velocity;
    if (direction == DOWN)
        moveVec -= WorldUp * velocity;

    parentObject->transform.position += moveVec * (isBoosting ? 2.0f : 1.0f);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch, float zoom)
{
    float yaw   = glm::radians(-xoffset * MouseSensitivity);
    float pitch = glm::radians(yoffset * MouseSensitivity);

    glm::quat yawRotation   = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat pitchRotation = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));

    parentObject->transform.rotation = yawRotation * parentObject->transform.rotation;
    parentObject->transform.rotation = parentObject->transform.rotation * pitchRotation;

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

Camera* Camera::mainCamera;
glm::vec3 Camera::WorldUp;