#include <Math/Math.h>

namespace PetrichorEngine::Math {
    float Math::ApplyEasing(float _x, EasingFunction _easingFunction) {
        _x = glm::clamp(_x, 0.0f, 1.0f);
        switch (_easingFunction) {
            case EasingFunction::IN_SINE: return 1 - glm::cos((_x * glm::pi<float>()) / 2);
            case EasingFunction::OUT_SINE: return glm::sin((_x * glm::pi<float>()) / 2);
            case EasingFunction::IN_OUT_SINE: return -(glm::cos(glm::pi<float>() * _x) - 1) / 2;
            case EasingFunction::IN_QUAD: return _x * _x;
            case EasingFunction::OUT_QUAD: return 1 - (1 - _x) * (1 - _x);
            case EasingFunction::IN_OUT_QUAD: return (_x < 0.5f) ? (2 * _x * _x) : (1 - glm::pow(-2 * _x + 2, 2) / 2);
            case EasingFunction::IN_CUBIC: return _x * _x * _x;
            case EasingFunction::OUT_CUBIC: return 1 - glm::pow(1 - _x, 3);
            case EasingFunction::IN_OUT_CUBIC: return (_x < 0.5f) ? (4 * _x * _x * _x) : (1 - glm::pow(-2 * _x + 2, 3) / 2);
            case EasingFunction::IN_QUART: return _x * _x * _x * _x;
            case EasingFunction::OUT_QUART: return 1 - glm::pow(1 - _x, 4);
            case EasingFunction::IN_OUT_QUART: return (_x < 0.5f) ? (8 * _x * _x * _x * _x) : (1 - glm::pow(-2 * _x + 2, 4) / 2);
            case EasingFunction::IN_QUINT: return _x * _x * _x * _x * _x;
            case EasingFunction::OUT_QUINT: return 1 - glm::pow(1 - _x, 5);
            case EasingFunction::IN_OUT_QUINT: return (_x < 0.5f) ? (16 * _x * _x * _x * _x * _x) : (1 - glm::pow(-2 * _x + 2, 5) / 2);
            case EasingFunction::IN_EXPO: return (_x == 0.0f) ? 0.0f : glm::pow(2, 10 * (_x - 1));
            case EasingFunction::OUT_EXPO: return (_x == 1.0f) ? 1.0f : 1 - glm::pow(2, -10 * _x);
            case EasingFunction::IN_OUT_EXPO: return (_x == 0.0f || _x == 1.0f) ? _x : (_x < 0.5f) ? (glm::pow(2, (20 * _x) - 10) / 2) : (2 - glm::pow(2, (-20 * _x) + 10)) / 2;
            case EasingFunction::IN_CIRC: return 1 - glm::sqrt(1 - _x * _x);
            case EasingFunction::OUT_CIRC: return glm::sqrt(1 - glm::pow(_x - 1, 2));
            case EasingFunction::IN_OUT_CIRC: return (_x < 0.5f) ? (1 - glm::sqrt(1 - 4 * _x * _x)) / 2 : (glm::sqrt(1 - glm::pow(-2 * _x + 2, 2)) + 1) / 2;
            case EasingFunction::IN_BACK: return _x * _x * _x - _x * glm::sin(_x * glm::pi<float>());
            case EasingFunction::OUT_BACK: return 1 - (_x - 1) * (_x - 1) * (_x - 1) - glm::sin((_x - 1) * glm::pi<float>());
            case EasingFunction::IN_OUT_BACK: return (_x < 0.5f) ? (2 * _x * _x * _x - 2 * _x * glm::sin(_x * glm::pi<float>())) / 2 : (1 - (2 * (_x - 1) * (_x - 1) * (_x - 1) - glm::sin((2 * (_x - 1)) * glm::pi<float>())) / 2);
            case EasingFunction::IN_ELASTIC: return glm::pow(2, 10 * (_x - 1)) * glm::sin((_x - 1) * 10 * glm::pi<float>());
            case EasingFunction::OUT_ELASTIC: return glm::pow(2, -10 * _x) * glm::sin((_x - 1) * 10 * glm::pi<float>()) + 1;
            case EasingFunction::IN_OUT_ELASTIC: return (_x < 0.5f) ? (glm::pow(2, 20 * _x - 10) * glm::sin((20 * _x - 10) * glm::pi<float>())) / 2 : 1 - glm::pow(2, -20 * (_x - 0.5f)) * glm::sin((20 * (_x - 0.5f) - 10) * glm::pi<float>());
            case EasingFunction::IN_BOUNCE: return 1 - glm::pow(1 - _x, 2);
            case EasingFunction::OUT_BOUNCE: return glm::pow(_x, 2);
            case EasingFunction::IN_OUT_BOUNCE: return (_x < 0.5f) ? (glm::pow(2 * _x, 2) / 2) : (1 - glm::pow(2 * (1 - _x), 2) / 2);
            default: return _x;
        }
    }
    float Math::Lerp(float _start, float _end, float _step)
    {
        return (_start * (1 - _step) + (_end * _step));
    }
    Vector4 Math::Lerp(Vector4 _start, Vector4 _end, float _step)
    {
        return Vector4(Lerp(_start.x, _end.x, _step), Lerp(_start.y, _end.y, _step), Lerp(_start.z, _end.z, _step), Lerp(_start.w, _end.w, _step));
    }
}
