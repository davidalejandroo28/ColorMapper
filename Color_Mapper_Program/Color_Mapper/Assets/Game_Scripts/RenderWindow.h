#pragma once

#include <iostream>

#include <glad/glad.h>
#include <SFML/System.hpp>

#include "../../Libraries/delaunator.hpp"
#include <glm/glm.hpp>

#include "Runtime_Script.h"
#include "Delaunay_Triangulation.h"

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

    template<typename type, int dataSize>
    array<type, dataSize> ConvertVertexData(int stride)
    {
        //Gotta put it in this format:
        //float vertexs[] = {
        //    //positions          //colors          
        //    //Top
        //    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, //BL
        //    0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, //BR
        //    -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f, //TL

        //    0.5f, -0.5f, 0.0f,  0.5f, 0.5f, 0.5f, //BR
        //    -0.5f, 0.5f, 0.0f,  0.5f, 0.5f, 0.5f, //TL
        //    0.5f, 0.5f, 0.0f,   0.5f, 0.5f, 0.5f, //TR
        //};

        array<type, dataSize> vertexs;

        int triangleIndex = 0;
        for (int x = 0; x < dataSize; x += stride * 3)
        {
            if (triangleIndex >= mesh.triangles.size())
            {
                break;
            }

            //add each triangles vertex
            for (int v = 0; v < 3; v++)
            {
                vertexs[x + v * stride] = mesh.vertices.at(mesh.triangles.at(triangleIndex).vertices[v]).cordinates[0] / (float)(abs(mesh.xAxisRange[0]) + abs(mesh.xAxisRange[1])) * 2;
                vertexs[x + v * stride + 1] = mesh.vertices.at(mesh.triangles.at(triangleIndex).vertices[v]).cordinates[1] / (float)(abs(mesh.yAxisRange[0]) + abs(mesh.yAxisRange[1])) * 2;
                vertexs[x + v * stride + 2] = 0.0f;
     
                vertexs[x + v * stride + 3] = (float) mesh.triangles.at(triangleIndex).color[0] / 255;
                vertexs[x + v * stride + 4] = (float) mesh.triangles.at(triangleIndex).color[1] / 255;
                vertexs[x + v * stride + 5] = (float) mesh.triangles.at(triangleIndex).color[2] / 255;
            }
        
            triangleIndex++;
        }

        return vertexs;
    }

    template<typename type, int dataSize>
    array<type, dataSize> ConvertIndiceData(int stride)
    {
        array<type, dataSize> indices;

        int triangle = 0;
        for (int tri = 0; tri < dataSize; tri += stride)
        {

            indices[triangle] = triangle;
            
            triangle++;
        }

        return indices;
    }

    //returns 2 points of a line
    array<float, 4> ReturnLinePoint(Vertex parentVert, Vertex childVert, bool useAxis)
    {
        if (useAxis)
        {
            //X-Axis
            if (parentVert.cordinates[1] > childVert.cordinates[1])
            {
                //create the topmost  point                                 //create bottom intersection point
                return { childVert.cordinates[0], (float) mesh.yAxisRange[1], childVert.cordinates[0], parentVert.cordinates[1] };
            }
            else
            {
                return { childVert.cordinates[0], parentVert.cordinates[1], childVert.cordinates[0], (float)mesh.yAxisRange[0] };
            }
        }
        else
        {
            //Y-Axis
            //if parent is right of child
            if (parentVert.cordinates[0] > childVert.cordinates[0])
            {
                //create the leftmost point                                 //create right intersection point
                return { (float)mesh.xAxisRange[0], childVert.cordinates[1], parentVert.cordinates[0], childVert.cordinates[1] };
            }
            else
            {
                return { parentVert.cordinates[0], childVert.cordinates[1], (float)mesh.xAxisRange[1], childVert.cordinates[1] };
            }
        }
    }

    void levelOrderTraversal(vector<array<float, 4>>& pointVals, int parentIndex, int midIndex, int boundryIndex, bool useAxis = 0)
    {
        pointVals.push_back(ReturnLinePoint(mesh.vertices.at(parentIndex), mesh.vertices.at(midIndex), useAxis));

        if (abs(parentIndex - midIndex) == 1 || abs(boundryIndex - midIndex) == 1)
        {
            return;
        }

        levelOrderTraversal(pointVals, midIndex, ReturnMidIndex(parentIndex, midIndex), boundryIndex, !useAxis);
        levelOrderTraversal(pointVals, midIndex, ReturnMidIndex(boundryIndex, midIndex), boundryIndex, !useAxis);
    }

    template<typename type, int dataSize>
    array<type, dataSize> ConvertPartitionData(int stride)
    {
        //Go through the KD tree
        array<type, dataSize> partitionData;
        vector<array<float, 4>> lines;

        bool useAxis = 1;
        int midIndex = ReturnMidIndex(0, mesh.vertices.size());
        int midLeftIndex = ReturnMidIndex(0, midIndex);
        int midRightIndex = ReturnMidIndex(midIndex, mesh.vertices.size());

        //ROOT
        lines.push_back({mesh.vertices[midIndex].cordinates[0], (float) mesh.yAxisRange[0], mesh.vertices[midIndex].cordinates[0],(float)mesh.yAxisRange[1]});

        levelOrderTraversal(lines, midIndex, midRightIndex, mesh.vertices.size(), useAxis);
        levelOrderTraversal(lines, midIndex, midLeftIndex, 0, useAxis);

        int line = 0;
        for (int x = 0; x <= dataSize - stride; x += stride)
        {
            if (line >= lines.size())
            {
                break;
            }
             
            partitionData[x] = lines.at(line)[0] / (float)(abs(mesh.xAxisRange[0]) + abs(mesh.xAxisRange[1])) * 2;
            partitionData[x + 1] = lines.at(line)[1] / (float)(abs(mesh.yAxisRange[0]) + abs(mesh.yAxisRange[1])) * 2;
            partitionData[x + 2] = 0.1f; //Z

            //color
            partitionData[x + 3] = 1;
            partitionData[x + 4] = 1;
            partitionData[x + 5] = 1;

            partitionData[x + 6] = lines.at(line)[2] / (float)(abs(mesh.xAxisRange[0]) + abs(mesh.xAxisRange[1])) * 2;
            partitionData[x + 7] = lines.at(line)[3] / (float)(abs(mesh.yAxisRange[0]) + abs(mesh.yAxisRange[1])) * 2;
            partitionData[x + 8] = 0.1f; //Z

            //color
            partitionData[x + 9] = 1;
            partitionData[x + 10] = 1;
            partitionData[x + 11] = 1;

            line++;
        }
        
        return partitionData;
    }

    static const int partitionStride = 12;
    static const int partitionSize = (DATASIZE / 3) * partitionStride;
    array<float, partitionSize> partitionData {}; 

    static const int stride = 6;
    static const int verticeSize = DATASIZE * stride * 3;
    array<float, verticeSize> vertexs {};

    static const int indiceStride = 3;
    static const int indiceSize = (DATASIZE / 2) * indiceStride;
    array<unsigned int, indiceSize> indices {};

    void InitializeRender()
    {
        glEnable(GL_DEPTH_TEST);

        CreateShaders();

        //DEBUG partition lines
        //partitionData = ConvertPartitionData<float, partitionSize>(partitionStride);

        //Verted data
        vertexs = ConvertVertexData<float, verticeSize>(stride);
        CreateBuffer((float(&)[verticeSize]) vertexs, GL_ARRAY_BUFFER);
        //Bind the data for the element array buffer
        CreateVertexArray();
    
        //This makes shapes out of the vertexs provided
        //indices = ConvertIndiceData<unsigned int, indiceSize>(indiceStride);        
        //unsigned int indiceElementBuffer = CreateBuffer((unsigned int(&)[indiceSize]) indices, GL_ELEMENT_ARRAY_BUFFER);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceElementBuffer); 
    }

    void Render()
    {
        CheckErrors();

        //Render triangle points and give buffer to element array
        CreateBuffer((float(&)[verticeSize]) vertexs, GL_ARRAY_BUFFER);
        CreateVertexArray();
        glDrawArrays(GL_TRIANGLES, 0, DATASIZE * 6);

        //CreateBuffer((float(&)[verticeSize]) vertexs, GL_ARRAY_BUFFER);
        //CreateVertexArray();
        //glDrawArrays(GL_POINTS, 0, DATASIZE * 6);

        //render triangles
        //CreateBuffer((unsigned int(&)[indiceSize]) indices, GL_ELEMENT_ARRAY_BUFFER);
        //glDrawElements(GL_TRIANGLES, mesh.triangles.size() * 3, GL_UNSIGNED_INT, 0);

        //Render Debug partition lines
        //CreateBuffer((float(&)[partitionSize]) partitionData, GL_ARRAY_BUFFER);
        //CreateVertexArray();
        //glDrawArrays(GL_LINES, 0, (DATASIZE / 3) * 12);
    }

    MeshData mesh;

	void Start()
	{
        //Get user Input

        //generate a random mesh to colorize
        mesh = GenerateRandomMesh();
        printMesh(mesh);

        //colorize mesh
        vector<double> cordinates;

        
        for (int x = 0; x < mesh.vertices.size(); x += 1)
        {
            cordinates.push_back(mesh.vertices[x].cordinates[0]);
            cordinates.push_back(mesh.vertices[x].cordinates[1]);

      
        }

        delaunator::Delaunator del(cordinates);
        for (int tri = 0; tri < del.triangles.size(); tri += 3)
        {
            Triangle triangle;
            triangle.vertices[0] = del.triangles[tri];
            triangle.vertices[1] = del.triangles[tri + 1];
            triangle.vertices[2] = (del.triangles[tri + 2]);

            array<int, 3> randomColor = { ((float)rand() / RAND_MAX) * 255, ((float)rand() / RAND_MAX) * 255, ((float)rand() / RAND_MAX) * 255 };
            triangle.color = randomColor;
                
            mesh.triangles.push_back(triangle);
        }
        
        printMesh(mesh);

        InitializeRender();
	}

	void Update()
	{
        Render();

        CheckErrors();
	}
};