#pragma once

#include "ECS/Component.h"
#include "Math/Math.h"

namespace PetrichorEngine::Scene
{
    class Camera : public ECS::Component
    {
    private:
        void updateCameraVectors();
    public:
        Math::Vector3 Front;
        Math::Vector3 Up;
        Math::Vector3 Right;
        Math::Vector4 backgroundColor;
        static Math::Vector3 WorldUp;

        static Camera* main;
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;
        bool isBoosting;
        bool perspective;

        ~Camera();
        Camera(bool _perspective = true, bool isMain = false);
        Math::Matrix4x4 GetViewMatrix();
        Math::Matrix4x4 GetProjectionMatrix(int screenWidth, int screenHeight);
        // void MoveCamera(Camera_Movement direction, float deltaTime);
        void FixedUpdate() override;
        // void ProcessMouseScroll(float yoffset);
    };
}
