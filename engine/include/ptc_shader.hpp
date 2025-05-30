#ifndef PTC_SHADER_HPP
#define PTC_SHADER_HPP
#define SHADER_PATH "resources/shaders/"

#include <glad/glad.h>

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	static std::vector<std::string> shaderCombinedPaths;
	static GLint activeShaderProgram;
public:
	static std::vector<Shader> shaders;
	static GLuint CommonFunctionsShaderID;
	GLuint ShaderProgramID;
	bool renderAsWireframe;

	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setUInt(const std::string& name, GLuint value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloat2(const std::string& name, glm::vec2 value) const;
	void setFloat3(const std::string& name, glm::vec3 value) const;
	void setFloat4(const std::string& name, glm::vec4 value) const;
	void setFloat1v(const std::string& name, GLsizei count, std::vector<float> value) const;
	void setFloat3v(const std::string& name, GLsizei count, std::vector<glm::vec3> value) const;
	void setMatrix4x4(const std::string& name, glm::mat4 value) const;
	static void SetCommonFunctionsShader(const char* commonShaderPath);
};

#endif