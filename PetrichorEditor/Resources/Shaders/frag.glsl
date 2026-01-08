#version 330 core

in vec4 fragColor;   // from vertex shader
out vec4 FragColor;  // output color

void main()
{
    FragColor = fragColor;
}
