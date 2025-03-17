#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;
uniform vec2 screenSize;
out vec4 vCol;

void main()
{
    // vec2 ndcPos = vec2(
    //     (aPos.x / screenSize.x) * 2.0f - 1.0f,
    //     1.0 - (aPos.y / screenSize.y) * 2.0f
    // );

    gl_Position = vec4((aPos.x / screenSize.x) * 2.0f - 1.0f, (aPos.y / screenSize.y) * 2.0f - 1.0f, aPos.z, 1.0f);
    vCol = aCol;
}