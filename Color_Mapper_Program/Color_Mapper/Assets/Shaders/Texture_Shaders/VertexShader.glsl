#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTextCord;

uniform mat4 transformationMatrix;
uniform mat4 worldTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

out vec2 textCord;
out vec4 vertexColor;

void main()
{
    gl_Position = projectionTransform * viewTransform * worldTransform * transformationMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = vec4(aCol.xyz, 1.0);
    textCord = aTextCord;
};