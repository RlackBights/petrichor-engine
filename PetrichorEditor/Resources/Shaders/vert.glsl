#version 330 core

layout (location = 0) in vec3 aPos; // x, y in screen coords, z for depth
layout (location = 1) in vec4 aCol; // vertex color

uniform vec2 screenSize; // screen width, height

out vec4 fragColor; // pass to fragment shader

void main()
{
    fragColor = aCol;

    // Convert screen coordinates to Normalized Device Coordinates
    float x = (aPos.x / screenSize.x) * 2.0 - 1.0;
    float y = (aPos.y / screenSize.y) * 2.0;
    gl_Position = vec4(x, y, aPos.z, 1.0);
}