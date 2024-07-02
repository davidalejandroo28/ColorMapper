#version 330 core

uniform float time;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec4 vertexColor;
in vec2 textCord;

out vec4 FragColor;

void main()
{
    FragColor = mix(texture(texture1, textCord), 
                    texture(texture2, textCord), 0.6)
    - 0.5 * vec4(sin(radians(180.0) * vertexColor.r + time), 
                     cos(radians(180.0) * vertexColor.g + time), 
                     sin(radians(180.0) * vertexColor.b + time - radians(180.0)), 
                     1.0);
};