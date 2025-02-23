#version 330 core

uniform sampler2D Texture;
uniform vec4 baseCol;
in vec2 TexCoord;
out vec4 FragColor;

void main()
{
	FragColor = baseCol * texture(Texture, TexCoord);
}