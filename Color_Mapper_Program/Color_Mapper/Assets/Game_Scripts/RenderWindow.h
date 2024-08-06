#pragma once

#include <iostream>

#include <glad/glad.h>
#include <SFML/System.hpp>
#include <unordered_map>

#include "Runtime_Script.h"
#include "Delaunay_Triangulation.h"
#include "Graph.h"
#include "Hashtable.h"


using namespace std;
class RenderWindow: public RuntimeScript
{
private:
    //--Variables for rendering--
    static const int stride = 5;
    static const int verticeSize = DATASIZE * stride * 3;
    array<float, verticeSize>* vertexs = new array<float, verticeSize>();

    //--Variables for colorizing
    MeshData* mesh;
    Graph graph;
    vector<Color> availableColors;

    //--Variables for user input
    bool tookUserInput = false;
    bool colorGraph = false;
    bool colorHashTable = false;
    bool doRunTime = false;

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
    unsigned int CreateBuffer(FLOAT(*bufferData)[SZ], GLenum&& bufferType)
    {
        unsigned int bufferId;

        glGenBuffers(1, &bufferId);
        glBindBuffer(bufferType, bufferId);
        glBufferData(bufferType, sizeof(*bufferData), bufferData, GL_STATIC_DRAW); //add data to the buffer

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
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        //color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        return vertexArray;
    }

    template<typename type, int dataSize>
    array<type, dataSize>* ConvertVertexData(int stride)
    {
        //Gotta put it in this format:
        //float vertexs[] = {
        //    //positions          //colors          
        //    //Top
        //    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, //BL
        //    0.5f, -0.5f,  1.0f, 1.0f, 1.0f, //BR
        //    -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, //TL

        //    0.5f, -0.5f,  0.5f, 0.5f, 0.5f, //BR
        //    -0.5f, 0.5f,  0.5f, 0.5f, 0.5f, //TL
        //    0.5f, 0.5f,   0.5f, 0.5f, 0.5f, //TR
        //};

        array<type, dataSize>* vertexs = new array<type, dataSize>();

        int triangleIndex = 0;
        for (int x = 0; x < dataSize; x += stride * 3)
        {
            if (triangleIndex >= mesh->triangles.size())
            {
                break;
            }

            //add each triangles vertex
            for (int v = 0; v < 3; v++)
            {
                vertexs->at(x + v * stride) = mesh->vertices.at(mesh->triangles.at(triangleIndex).vertices[v]).cordinates[0] / (float)(abs(mesh->xAxisRange[0]) + abs(mesh->xAxisRange[1])) * 2;
                vertexs->at(x + v * stride + 1) = mesh->vertices.at(mesh->triangles.at(triangleIndex).vertices[v]).cordinates[1] / (float)(abs(mesh->yAxisRange[0]) + abs(mesh->yAxisRange[1])) * 2;


                vertexs->at(x + v * stride + 2) = (float)mesh->triangles.at(triangleIndex).color[0] / 255;
                vertexs->at(x + v * stride + 3) = (float)mesh->triangles.at(triangleIndex).color[1] / 255;
                vertexs->at(x + v * stride + 4) = (float)mesh->triangles.at(triangleIndex).color[2] / 255;
            }

            triangleIndex++;
        }

        return vertexs;
    }

    void InitializeRender()
    {
        glEnable(GL_DEPTH_TEST);

        CreateShaders();

        //Verted data
        if (vertexs != nullptr)
            delete[] vertexs;
        vertexs = ConvertVertexData<float, verticeSize>(stride);
        CreateBuffer((float(*)[verticeSize]) vertexs, GL_ARRAY_BUFFER);
        //Bind the data for the element array buffer
        CreateVertexArray();
    }

    void Render()
    {
        CheckErrors();

        //Render triangle points and give buffer to element array
        CreateBuffer((float(*)[verticeSize]) vertexs, GL_ARRAY_BUFFER);
        CreateVertexArray();
        glDrawArrays(GL_TRIANGLES, 0, DATASIZE * 6);
    }
public:
    vector<Color> GetUserInput()
    {
        //Get user Input (DON'T CHANGE)
        unordered_map<string, Color> options = { {"Red", Color(255, 0, 0)}, {"Blue", Color(0, 255, 0)}, {"Green", Color(0, 0, 255)},  
            {"Yellow", Color(255, 251, 0)}, {"Cyan", Color(0, 234, 255)}, {"Magenta", Color(248, 0, 255)}, {"Orange", Color(255, 127, 0)},
            {"BabyBlue", Color(137, 207, 240)}, {"Indigo", Color(75, 0, 130)}, {"Violet", Color(148, 0, 211)}, 
            {"Lime", Color(128, 255, 0)}, {"Rose", Color(235, 52, 100)}, {"Sand", Color(173, 145, 66)} };

        vector<Color> availableColors;


        cout << "Project 3: Mapping with Colors" << endl;
      
        cout << endl << "What colors do you want?" << endl;
        cout << "Options are: (say DONE to move onto the next step)" << endl;
        for (auto names = options.begin(); names != options.end(); names++)
        {
            cout << names->first << ", " << flush;
        }

        cout << endl << "Now input your color(s): " << endl;

        //TODO add error handling
        while (true)
        {
            string input;
            try
            {
                cin >> input;

                if (input == "DONE")
                {
                    break;
                }

                availableColors.push_back(options.at(input));
            }
            catch (...)
            {
                cout << "Error, you might've mispelled a word" << endl << endl;
            }
        }

        cout << endl << "Which algorithm do you want to run?" << endl;
        cout << "Graph or Hash_Table Colorize Function?" << endl;

        while (true)
        {
            string input;
            try
            {
                cin >> input;

                if (input == "Graph")
                {
                    colorGraph = true;
                    cout << "This might take 10 mins to setup" << endl;
                    graph.setNeighbors(mesh->triangles);
                    break;
                }
                else if (input == "Hash_Table")
                {
                    colorHashTable = true;
                    cout << "This might take 10 mins" << endl;
                    break;
                }
            }
            catch (...)
            {
                cout << "Error, you might've mispelled a word" << endl << endl;
            }
        }

        cout << "Would you like do view the algorithm in real time? (Yes or No)" << endl;
        while (true)
        {
            string input;
            try
            {
                cin >> input;

                if (input == "Yes")
                {
                    doRunTime = true;
                    graph.enableRealTime();
                    break;
                }
                else if (input == "No")
                {
                    if (colorHashTable)
                        ColorizeHTable(*mesh, availableColors);
                    doRunTime = false;
                    graph.disableRealTime();
                    break;
                }
            }
            catch (...)
            {
                cout << "Error, you might've mispelled a word" << endl << endl;
            }
        }

        return availableColors;
    }
    
	void Start()
	{
        //generate a random mesh to colorize
        cout << "----Generating mesh----" << endl;

        mesh = GenerateRandomMesh();
        
        cout << endl << "----Rendering graph----" << endl;
        InitializeRender();
        Render();
	}

	void Update()
	{
        if (!tookUserInput)
        {
            //colorize mesh
            cout << endl << "----Coloring graph---- " << endl;

            availableColors = GetUserInput();

            tookUserInput = true;

            InitializeRender();
        }

        if (colorGraph)
        { 
            if (graph.doRealTime() == false)
            {
                graph.colorMesh(*mesh, availableColors);
                colorGraph = false;
            }
            else
            {
                //Do algorithm in real time!
                graph.colorMesh(*mesh, availableColors);
            }
           
            InitializeRender();
        }

        if (colorHashTable && doRunTime)
        {
            //Color the hash table and Do algorithm in real time!
            ColorizeHTable(*mesh, availableColors, true);
            InitializeRender();
        }

        Render();

        CheckErrors();
	}

    void DeleteResources()
    {
        delete[] vertexs;
        mesh->DeleteResources();
        delete mesh;
    }
};