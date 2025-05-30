#include "ptc_console.hpp"
#include "ptc_file_reader.hpp"
#include <ptc_shader.hpp>

Shader::Shader()
{
	ShaderProgramID = 1;
	renderAsWireframe = false;
}
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	renderAsWireframe = false;
	for (int i = 0; i < shaderCombinedPaths.size(); i++)
	{
		if (shaderCombinedPaths[i] == std::string(vertexPath) + std::string(fragmentPath))
		{
			ShaderProgramID = shaders[i].ShaderProgramID;
			return;
		}
	}

	// Retrieve shader source codes from file path
	std::string fullVertexPath = SHADER_PATH + std::string(vertexPath);
	std::string fullFragmentPath = SHADER_PATH + std::string(fragmentPath);

	std::string vertexCode = FileReader::Read(fullVertexPath);
	std::string fragmentCode = FileReader::Read(fullFragmentPath);

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Compile Shaders
	int success;
	char infoLog[512];

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		Console::WriteLine(Console::FormatString("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n%s", vertexPath, infoLog));
	};

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		Console::WriteLine(Console::FormatString("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n%s", fragmentPath, infoLog));
	};

	ShaderProgramID = glCreateProgram();

	glAttachShader(ShaderProgramID, CommonFunctionsShaderID);
	glAttachShader(ShaderProgramID, vertex);
	glAttachShader(ShaderProgramID, fragment);
	glLinkProgram(ShaderProgramID);

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, infoLog);
		Console::WriteLine(Console::FormatString("ERROR::SHADER::PROGRAM::LINKING_FAILED\n", infoLog));
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	shaders.push_back(*this);
	shaderCombinedPaths.push_back(std::string(vertexPath) + std::string(fragmentPath));
	use();
}
void Shader::use()
{
	glGetIntegerv(GL_CURRENT_PROGRAM, &activeShaderProgram);
	if (activeShaderProgram == ShaderProgramID) return;
	glUseProgram(ShaderProgramID);
}
void Shader::setBool(const std::string& name, bool value) const
{
	GLint uniformLocation = glGetUniformLocation(ShaderProgramID, name.c_str());
	if (uniformLocation != -1) glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	GLint uniformLocation = glGetUniformLocation(ShaderProgramID, name.c_str());
	if (uniformLocation != -1) glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}
void Shader::setUInt(const std::string& name, GLuint value) const
{
	GLint uniformLocation = glGetUniformLocation(ShaderProgramID, name.c_str());
	if (uniformLocation != -1) glUniform1ui(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	GLint uniformLocation = glGetUniformLocation(ShaderProgramID, name.c_str());
	if (uniformLocation != -1) glUniform1f(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}
void Shader::setFloat2(const std::string& name, glm::vec2 value) const
{
	GLint uniformLocation = glGetUniformLocation(ShaderProgramID, name.c_str());
	if (uniformLocation != -1) glUniform2f(glGetUniformLocation(ShaderProgramID, name.c_str()), value.x, value.y);
}
void Shader::setFloat3(const std::string& name, glm::vec3 value) const
{
	GLint uniformLocation = glGetUniformLocation(ShaderProgramID, name.c_str());
	if (uniformLocation != -1) glUniform3f(glGetUniformLocation(ShaderProgramID, name.c_str()), value.x, value.y, value.z);
}
void Shader::setFloat4(const std::string& name, glm::vec4 value) const
{
	GLint uniformLocation = glGetUniformLocation(ShaderProgramID, name.c_str());
	if (uniformLocation != -1) glUniform4f(glGetUniformLocation(ShaderProgramID, name.c_str()), value.x, value.y, value.z, value.w);
}
void Shader::setFloat3v(const std::string& name, GLsizei count, std::vector<glm::vec3> value) const
{
	GLint uniformLocation = glGetUniformLocation(ShaderProgramID, name.c_str());
	if (uniformLocation != -1) glUniform3fv(glGetUniformLocation(ShaderProgramID, name.c_str()), count, glm::value_ptr(value[0]));
}
void Shader::setFloat1v(const std::string& name, GLsizei count, std::vector<float> value) const
{
	GLint uniformLocation = glGetUniformLocation(ShaderProgramID, name.c_str());
	if (uniformLocation != -1) glUniform1fv(glGetUniformLocation(ShaderProgramID, name.c_str()), count, value.data());
}
void Shader::setMatrix4x4(const std::string& name, glm::mat4 value) const
{
	GLint uniformLocation = glGetUniformLocation(ShaderProgramID, name.c_str());
	if (uniformLocation != -1) glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::SetCommonFunctionsShader(const char* commonShaderPath)
{
	std::string commonFunctionsCode = FileReader::Read(SHADER_PATH + std::string(commonShaderPath));
	const char* cShaderCode = commonFunctionsCode.c_str();

	// Compile the common shader functions
	int success;
	char infoLog[512];

	CommonFunctionsShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(CommonFunctionsShaderID, 1, &cShaderCode, NULL);
	glCompileShader(CommonFunctionsShaderID);
	Console::Write("---");

	glGetShaderiv(CommonFunctionsShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(CommonFunctionsShaderID, 512, NULL, infoLog);
		Console::WriteLine(Console::FormatString("ERROR::SHADER::COMMON_SHADER::COMPILATION_FAILED\n%s", infoLog));
	}
	Console::Write("---");
}

GLuint Shader::CommonFunctionsShaderID;
GLint Shader::activeShaderProgram;
std::vector<Shader> Shader::shaders;
std::vector<std::string> Shader::shaderCombinedPaths;