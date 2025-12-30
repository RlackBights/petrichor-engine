#pragma once

#include "ECS/Component.h"
#include "Math/Math.h"

namespace PetrichorEngine
{
    class Camera : public Component
    {
    private:
        void updateCameraVectors();
    public:
        Vector3 Front;
        Vector3 Up;
        Vector3 Right;
        Vector4 backgroundColor;
        static Vector3 WorldUp;

        static Camera* main;
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;
        bool isBoosting;
        bool perspective;

        ~Camera();
        Camera(bool _perspective = true, bool isMain = false);
        Matrix4x4 GetViewMatrix();
        Matrix4x4 GetProjectionMatrix(int screenWidth, int screenHeight);
        // void MoveCamera(Camera_Movement direction, float deltaTime);
        void FixedUpdate() override;
        // void ProcessMouseScroll(float yoffset);
    };
}
