#version 330 core

out vec4 FragColor;
uniform vec2 quadSize;
uniform vec2 border;
in vec4 vCol;
in vec2 vPos;

void main()
{
    float borderSizeX = border.x / quadSize.x;
    float borderSizeY = border.y / quadSize.y;

    bool isBorder =
        vPos.x < borderSizeX || vPos.x > 1.0 - borderSizeX ||
        vPos.y < borderSizeY || vPos.y > 1.0 - borderSizeY;
    
    vec4 color = vCol;
    if (isBorder) color = vec4(vec3(0.0), 1.0);
    FragColor = color;
}