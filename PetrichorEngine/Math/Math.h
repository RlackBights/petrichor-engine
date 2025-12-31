#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PetrichorEngine::Math
{
    using Vector2 = glm::vec2;
    using Vector3 = glm::vec3;
    using Vector4 = glm::vec4;
    using Vector2i = glm::ivec2;
    using Vector3i = glm::ivec3;
    using Vector4i = glm::ivec4;

    using Matrix4x4 = glm::mat4x4;
    using Quaternion = glm::quat;

    enum EasingFunction {
        IN_SINE,
        OUT_SINE,
        IN_OUT_SINE,
        IN_QUAD,
        OUT_QUAD,
        IN_OUT_QUAD,
        IN_CUBIC,
        OUT_CUBIC,
        IN_OUT_CUBIC,
        IN_QUART,
        OUT_QUART,
        IN_OUT_QUART,
        IN_QUINT,
        OUT_QUINT,
        IN_OUT_QUINT,
        IN_EXPO,
        OUT_EXPO,
        IN_OUT_EXPO,
        IN_CIRC,
        OUT_CIRC,
        IN_OUT_CIRC,
        IN_BACK,
        OUT_BACK,
        IN_OUT_BACK,
        IN_ELASTIC,
        OUT_ELASTIC,
        IN_OUT_ELASTIC,
        IN_BOUNCE,
        OUT_BOUNCE,
        IN_OUT_BOUNCE
    };

    class Math
    {
    public:
        static float ApplyEasing(float _x, EasingFunction _easingFunction);
        static float Lerp(float _start, float _end, float _step);
        static glm::vec4 Lerp(glm::vec4 _start, glm::vec4 _end, float _step);
    };
}