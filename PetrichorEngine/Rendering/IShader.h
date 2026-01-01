#pragma once

#include "Math/Math.h"
#include <string>
#include <vector>

namespace PetrichorEngine::Rendering
{	
    class IShader
    {
    public:
		virtual void Use();
		virtual void SetBool(const std::string& name, bool value) const;
		virtual void SetInt(const std::string& name, int value) const;
		virtual void SetUInt(const std::string& name, unsigned int value) const;
		virtual void SetFloat(const std::string& name, float value) const;
		virtual void SetFloat2(const std::string& name, Math::Vector2 value) const;
		virtual void SetFloat3(const std::string& name, Math::Vector3 value) const;
		virtual void SetFloat4(const std::string& name, Math::Vector4 value) const;
		virtual void SetFloat1v(const std::string& name, int count, std::vector<float> value) const;
		virtual void SetFloat3v(const std::string& name, int count, std::vector<Math::Vector3> value) const;
		virtual void SetMatrix4x4(const std::string& name, Math::Matrix4x4 value) const;
    };
}