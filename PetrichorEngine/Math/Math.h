#pragma once

#include <glm/detail/qualifier.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PetrichorEngine::Math
{
    template <glm::length_t L, typename T>
    using TVector = glm::vec<L, T>;

    template <typename T = float>
    using TVector2 = TVector<2, T>;
    template <typename T = float>
    using TVector3 = TVector<3, T>;
    template <typename T = float>
    using TVector4 = TVector<4, T>;

    using Vector2 = TVector2<>;
    using Vector3 = TVector3<>;
    using Vector4 = TVector4<>;

    using Vector2i = TVector2<int>;
    using Vector3i = TVector3<int>;
    using Vector4i = TVector4<int>;

    template <glm::length_t C, glm::length_t R, typename T>
    using TMatrix = glm::mat<C, R, T>;

    using Matrix4x4 = TMatrix<4, 4, float>;

    
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