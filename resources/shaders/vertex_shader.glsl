#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform uint time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 fPos;
out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

float random(vec2 co);

void main()
{
	TexCoord = aTexCoord;
	fPos = projection * view * model * vec4(aPos.xyz, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0f));
	Normal = mat3(model) * aNormal;
	gl_Position = fPos;
}