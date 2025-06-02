#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;
uniform vec4 textColor;

void main()
{    
    float alpha = texture(text, TexCoords).a;
    if (alpha < 0.5) // Discard fully transparent fragments
        discard;
    FragColor = vec4(textColor.rgb, textColor.a * alpha);
    // FragColor = vec4(vec3(alpha), 1.0); // Show grayscale alpha
}  