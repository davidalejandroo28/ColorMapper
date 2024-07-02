#pragma once

#include <iostream>

#include <glad/glad.h>
#include <SFML/System.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "./System_Scripts/Runtime_Script.h"

using namespace std;
class Playground: public RuntimeScript
{
public:
    glm::mat4x4 transform = glm::mat4(1.0f);
    glm::mat4x4 worldTrans = glm::mat4(1.0f);
    glm::mat4x4 viewTrans = glm::mat4(1.0f);
    glm::mat4x4 projTrans = glm::mat4(1.0f);


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
        Shader vertexShader("./Assets/Shaders/Texture_Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
        Shader fragmentShader("./Assets/Shaders/Texture_Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);

        unsigned int shaderProgram = CreateShaderProgram(fragmentShader, vertexShader);

        shaderId = shaderProgram;

        glUseProgram(shaderProgram);
    }

    int CreateVertexArray()
    {
        unsigned int tri2VertexArray;
        glGenVertexArrays(1, &tri2VertexArray);
        glBindVertexArray(tri2VertexArray);

        //pos attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        //color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        //texture attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        return tri2VertexArray;
    }

    struct Image
    {
        int width;
        int height;
        int channels;

        unsigned char* imageData;

        ~Image()
        {
            stbi_image_free(imageData);
        }
    };

    Image LoadAImage(const char* imagePath, bool isFlipped = false)
    {
        Image image;

        if (isFlipped)
        {
            stbi_set_flip_vertically_on_load(true);
        }

        image.imageData = stbi_load(imagePath, &image.width, &image.height, &image.channels, 0);

        return image;
    }

    void AddTextures()
    {
        unsigned int textures[3];
        glGenTextures(3, textures);

        Image wallImage = LoadAImage("./Assets/Textures/wall.jpg");
        Image containerImage = LoadAImage("./Assets/Textures/container.jpg");
        Image faceImage = LoadAImage("./Assets/Textures/awesomeface.png", true);

        if (wallImage.imageData != nullptr && containerImage.imageData != nullptr && faceImage.imageData != nullptr)
        {
            glActiveTexture(GL_TEXTURE0 + textures[0]);
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wallImage.width, wallImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallImage.imageData);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glActiveTexture(GL_TEXTURE0 + textures[1]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, containerImage.width, containerImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, containerImage.imageData);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glActiveTexture(GL_TEXTURE0 + textures[2]);
            glBindTexture(GL_TEXTURE_2D, textures[2]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, faceImage.width, faceImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, faceImage.imageData);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            unsigned int texture2Loc = glGetUniformLocation(shaderId, "texture2");
            glUniform1i(texture2Loc, 3);
        }
        else
        {
            cout << "Failed to generate textures!" << endl;
            return;
        }
    }

    glm::vec3 cubePositions[10] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    float time = 0;
    void Render()
    {
        unsigned int timeLoc = glGetUniformLocation(shaderId, "time");
        glUniform1f(timeLoc, time);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            worldTrans = glm::rotate(worldTrans, glm::eulerAngles(glm::toQuat(transform))[0] + 0.1f, glm::vec3(1, 0, 0));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            worldTrans = glm::rotate(worldTrans, glm::eulerAngles(glm::toQuat(transform))[0] - 0.1f, glm::vec3(1, 0, 0));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            worldTrans = glm::rotate(worldTrans, glm::eulerAngles(glm::toQuat(transform))[0] + 0.1f, glm::vec3(0, 1, 0));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            worldTrans = glm::rotate(worldTrans, glm::eulerAngles(glm::toQuat(transform))[0] - 0.1f, glm::vec3(0, 1, 0));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            worldTrans = glm::translate(worldTrans, glm::vec3(0, 0, -1));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            worldTrans = glm::translate(worldTrans, glm::vec3(0, 0, 1));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            worldTrans = glm::translate(worldTrans, glm::vec3(0, -1, 0));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            worldTrans = glm::translate(worldTrans, glm::vec3(0, 1, 0));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            worldTrans = glm::translate(worldTrans, glm::vec3(1, 0, 0));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            worldTrans = glm::translate(worldTrans, glm::vec3(-1, 0, 0));
        }

        unsigned int transformMLoc = glGetUniformLocation(shaderId, "transformationMatrix");
        glUniformMatrix4fv(transformMLoc, 1, GL_FALSE, glm::value_ptr(transform));

        unsigned int viewTransLoc = glGetUniformLocation(shaderId, "viewTransform");
        glUniformMatrix4fv(viewTransLoc, 1, GL_FALSE, glm::value_ptr(viewTrans));

        unsigned int projectTransLoc = glGetUniformLocation(shaderId, "projectionTransform");
        glUniformMatrix4fv(projectTransLoc, 1, GL_FALSE, glm::value_ptr(projTrans));

        unsigned int texture1Loc = glGetUniformLocation(shaderId, "texture1");
        glUniform1i(texture1Loc, 1);


        for (int x = 0; x <= 10; x++)
        {
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

            unsigned int worldTransLoc = glGetUniformLocation(shaderId, "worldTransform");
            glUniformMatrix4fv(worldTransLoc, 1, GL_FALSE, glm::value_ptr(glm::translate(worldTrans, cubePositions[x])));
        }
        
    }

    void InitializeRender()
    {
        glEnable(GL_DEPTH_TEST);

        CreateShaders();

        float vertexs[] = {
            //positions          //colors          //Texture cordinates
            //Top
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //BL
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //BR
            -0.5f, 0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //TL
            0.5f, 0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //TR

            //Bottom (starting at 4)
            -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //BL
            0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //BR
            -0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //TL
            0.5f, 0.5f, 0.5f,   1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //TR
        };

        unsigned int indices[] = {
            //top plane
            0, 1, 2,
            3, 2, 1,

            //bottom plane
            4, 5, 6,
            7, 6, 5,

            //z plane (it's normal to)
            4, 1, 5,
            4, 1, 0,

            //-z plane
            6, 7, 3,
            6, 2, 3,

            //x plane
            5, 3, 1,
            5, 3, 7,

            //-x plane
            4, 2, 0,
            4, 2, 6,

        };

        unsigned int tri2VertexBuffer = CreateBuffer(vertexs, GL_ARRAY_BUFFER);
        unsigned int indiceElementBuffer = CreateBuffer(indices, GL_ELEMENT_ARRAY_BUFFER);

        int vertexArray = CreateVertexArray();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceElementBuffer);
       
        AddTextures();
        
        Render();
    }

	void Start()
	{
        worldTrans = glm::rotate(worldTrans, glm::radians(-55.0f), glm::vec3(1, 0, 0));
        viewTrans = glm::translate(viewTrans, glm::vec3(0, 0, -3.0f));
        projTrans = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        InitializeRender();
	}

	void Update()
	{
        Render();

        CheckErrors();
	}
};


// A useful Example
//void InitializeRender()
//{
//    const char* vertexShaderCode = "#version 330 core\n"
//        "layout (location = 0) in vec3 aPos;\n"
//        "void main()\n"
//        "{\n"
//        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//        "}\0";
//
//    const char* fragmentShaderCode = "#version 330 core\n"
//        "out vec4 FragColor;\n"
//        "void main()\n"
//        "{\n"
//        " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//        "}\0";
//
//    float vertices[] = {
//        0.0f, 0.0f, 0.0f, //BL
//        0.0f, 0.5f, 0.0f, //TL
//        0.5f, 0.0f, 0.0f, //BR
//        0.5f, 0.5f, 0.0f, //TR
//    };
//
//    unsigned int indices[] = {
//        0, 1, 2,
//        1, 2, 3
//    };
//
//    unsigned int vertexBuffer = CreateVerticeBuffer(vertices);
//    unsigned int indiceBuffer = CreateIndiceBuffer(indices);GL_ELEMENT_ARRAY_BUFFER
//    
//    //specifying how OpenGl should connect vertex data to the vertex shader:
//    unsigned int vertexArray;
//    glGenVertexArrays(1, &vertexArray);
//    glBindVertexArray(vertexArray);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //specify data input for Vertex array pointer
//    glEnableVertexAttribArray(0);
//
//    //create shaders
//    unsigned int shaderProgram = CreateShaderProgram(fragmentShaderCode, vertexShaderCode);
//    glUseProgram(shaderProgram);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
//    //glBindVertexArray(GL_VERTEX_ARRAY, vertexArray);
// 
// glBindVertexArray(tri1VertexArray);
// glDrawArrays(GL_TRIANGLES, 0, 3); 
// 
//}