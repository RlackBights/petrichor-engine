#version 330 core

layout (location = 0) in vec3 aPos; // pixel-space coords
layout (location = 1) in vec4 aCol;

uniform vec2 screenSize;
out vec4 vCol;

void main()
{
    float x = (aPos.x / screenSize.x) * 2.0 - 1.0;
    float y = 1.0 - (aPos.y / screenSize.y) * 2.0;

    gl_Position = vec4(x, y, aPos.z, 1.0);
    vCol = aCol;
}
