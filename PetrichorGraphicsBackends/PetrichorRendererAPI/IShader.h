#pragma once

#include <string>

namespace PetrichorRendererAPI
{	
    class IShader
    {
    public:
		virtual void Use();
		virtual void SetBool(const std::string& name, bool value) const;
		virtual void SetInt(const std::string& name, int value) const;
		virtual void SetUInt(const std::string& name, unsigned int value) const;
		virtual void SetFloat(const std::string& name, float value) const;
		virtual void SetFloat2(const std::string& name, float value[2]) const;
		virtual void SetFloat3(const std::string& name, float value[3]) const;
		virtual void SetFloat4(const std::string& name, float value[4]) const;
		virtual void SetMatrix4x4(const std::string& name, const float* value) const;
    };
}