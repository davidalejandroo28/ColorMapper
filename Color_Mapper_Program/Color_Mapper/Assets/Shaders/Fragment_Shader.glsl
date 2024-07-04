#version 330 core

uniform float time;

in vec4 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(sin(radians(180.0) * vertexColor.r + time), 
                     cos(radians(180.0) * vertexColor.g + time), 
                     sin(radians(180.0) * vertexColor.b + time - radians(180.0)), 
                     1.0);
};