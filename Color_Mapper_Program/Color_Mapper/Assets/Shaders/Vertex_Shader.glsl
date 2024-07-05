#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aPos.xyz, 1.0);
    vertexColor = vec4(aCol.xyz, 1.0);
};