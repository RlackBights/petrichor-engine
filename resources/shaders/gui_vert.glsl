#version 330 core

layout (location = 0) in vec3 aPos;
out vec4 vCol;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vCol = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}