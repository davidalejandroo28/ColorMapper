#pragma once

#include <iostream>

#include <glad/glad.h>
#include <SFML/System.hpp>

#include <glm/glm.hpp>
#include "./System_Scripts/Runtime_Script.h"

using namespace std;
class RenderWindow: public RuntimeScript
{
public:
    //This checks for any errors on the GPU and then converts the error code
    void CheckErrors()
    {
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR)
        {
            std::string error;
            switch (errorCode)
            {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            }

            cout << error << endl;
        }
    }

    template<typename FLOAT, size_t SZ>
    unsigned int CreateBuffer(FLOAT(&bufferData)[SZ], GLenum&& bufferType)
    {
        unsigned int bufferId;

        glGenBuffers(1, &bufferId);
        glBindBuffer(bufferType, bufferId);
        glBufferData(bufferType, sizeof(bufferData), bufferData, GL_STATIC_DRAW); //add data to the buffer

        return bufferId;
    }

    unsigned int CreateShaderProgram(Shader vertexShader, Shader fragmentShader)
    {
        //link shaders together to create a program pipeline
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader.GetId());
        glAttachShader(shaderProgram, fragmentShader.GetId());
        glLinkProgram(shaderProgram);

        return shaderProgram;
    }

    int shaderId = 0;
    void CreateShaders()
    {
        Shader vertexShader("./Assets/Shaders/Vertex_Shader.glsl", GL_VERTEX_SHADER);
        Shader fragmentShader("./Assets/Shaders/Fragment_Shader.glsl", GL_FRAGMENT_SHADER);

        unsigned int shaderProgram = CreateShaderProgram(fragmentShader, vertexShader);

        shaderId = shaderProgram;

        glUseProgram(shaderProgram);
    }

    int CreateVertexArray()
    {
        unsigned int vertexArray;
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        //pos attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        //color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        return vertexArray;
    }

    void InitializeRender()
    {
        glEnable(GL_DEPTH_TEST);

        CreateShaders();

        float vertexs[] = {
            //positions          //colors          
            //Top
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, //BL
            0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, //BR
            -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f, //TL

            0.5f, -0.5f, 0.0f,  0.5f, 0.5f, 0.5f, //BR
            -0.5f, 0.5f, 0.0f,  0.5f, 0.5f, 0.5f, //TL
            0.5f, 0.5f, 0.0f,   0.5f, 0.5f, 0.5f, //TR
        };

        //This makes shapes out of the vertexs provided
        unsigned int indices[] = {
            0, 1, 2,
            5, 2, 1,
        };

        unsigned int vertexBuffer = CreateBuffer(vertexs, GL_ARRAY_BUFFER);
        unsigned int indiceElementBuffer = CreateBuffer(indices, GL_ELEMENT_ARRAY_BUFFER);

        int vertexArray = CreateVertexArray();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceElementBuffer);     
    }

    void Render()
    {
        CheckErrors();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

	void Start()
	{
        InitializeRender();
	}

	void Update()
	{
        Render();

        CheckErrors();
	}
};