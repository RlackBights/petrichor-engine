#version 330 core

layout (location = 0) in vec3 aPos; // pixel-space coords
layout (location = 1) in vec4 aCol;

uniform vec2 screenSize;
uniform vec2 quadPos;
uniform vec2 quadSize;
out vec4 vCol;
out vec2 vPos;

void main()
{
    float x = (aPos.x / screenSize.x) * 2.0 - 1.0;
    float y = (aPos.y / screenSize.y) * 2.0 - 1.0;

    gl_Position = vec4(x, y, aPos.z, 1.0);
    vCol = aCol;
    vPos = (aPos.xy - quadPos) / quadSize;
}
