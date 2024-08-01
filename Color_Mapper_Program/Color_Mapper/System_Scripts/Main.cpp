////CONFINGURATIONS////
//To configure for visual studio, go to project->properties->preprocessor->preprocessor definitions->set 1 or 0's accordingly

#if TESTMODE 1
    
#else
//don't run this main if tests are enabled

    #if DEBUGMODE 1
        #include <vld.h>
    #endif

#pragma once

#define DATASIZE 1000

#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <algorithm>

#include <glad/glad.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "Shader.h"
#include "Event.h"
#include "System_Manager.h"

#include "../../Assets/Game_Scripts/RenderWindow.h"

using namespace std;

void DeleteScripts(vector<Observer<>*> scripts)
{
    for (Observer<>* observer: scripts)
    {
        delete observer;
    }
}

void RetrieveUserSettings()
{

}

int ShowWindow()
{
    sf::Context context;

    //Initialize glad
    if (!gladLoadGLLoader((GLADloadproc)(context.getFunction)))
    {
        cout << "Could not initialize GLAD!!!!" << endl;
        return -1;
    }
    //

    //Initialize the system
    SystemManager* sysManager = SystemManager::GetInstance();
    sysManager->PrintSystemInfomation();

    vector<Observer<>*> scripts = { new RenderWindow };

    // create the window
    sf::ContextSettings windowSettings(24);
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, windowSettings);
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        // clear the buffers
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Call scripts
        sysManager->UpdateSystemScripts();
        sysManager->UpdateGameScripts();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...
    DeleteScripts(scripts);
    sysManager->DeleteInstance();

    return 0;
}

struct Vertex
{
    //X cordinate is 0, Y cordinate is 1
    array<float, 2> cordinates;
    int triangleIndex;
};

struct Triangle
{
    array<int, 3> triNeigh;
    array<int, 3> vertices;
};

struct MeshData
{
    array<Vertex, DATASIZE> vertices;
    vector<Triangle> triangles;
};

//This sorts the vertex list into a 1D array kd-tree structure
//This uses quick sort
void SortVertexs(array<Vertex, DATASIZE>& vertices, int depth = 0, int lowIndex = 0, int highIndex = DATASIZE)
{
    //Return case to stop sorting
    if (lowIndex >= highIndex)
    {
        return;
    }

    //int midIndex = (lowIndex + highIndex) / 2;
    int axisMedian = 0;
    bool useYAxis = depth % 2;

    //find pivot to place in the center (the pivot is the middle most axis point)
    if (useYAxis)
    {
        for (int y = lowIndex; y < highIndex; y++)
        {
            //find middle point (it's m in:https://www.desmos.com/calculator/sbviponlyb)
            auto middleIter = vertices.begin() + ((highIndex - lowIndex) / 2 + lowIndex);
            nth_element(vertices.begin() + lowIndex, middleIter, vertices.begin() + highIndex); // find median and place it in the middle
        }
    }
    else
    {
       
    }
    

}

MeshData GenerateRandomMesh()
{
    //Generate random data set
    array<int, 2> xCordBounds = { -10, 10 };
    array<int, 2> yCordBounds = { -10, 10 };

    //make a seed (this should somehow be randomized)
    std::srand(1);

    MeshData mesh;

    //place each vertex randomly on a 2d plane
    for (Vertex vertex : mesh.vertices)
    {
        float randomVal = (float)rand() / RAND_MAX;

        //implementation of equation x or y = (distance of allowed bounds * random Value) + (left bound)
        //Proof: https://www.desmos.com/calculator/oat3knijz3
        vertex.cordinates[0] = ((xCordBounds[1] - xCordBounds[0]) * randomVal) + xCordBounds[0];
        vertex.cordinates[1] = ((yCordBounds[1] - yCordBounds[0]) * randomVal) + yCordBounds[0];
    }





    return mesh;
}

int main()
{
    //Get user Input

    //generate a random mesh to colorize
    MeshData mesh = GenerateRandomMesh();

    /*array<int, 11> test = { 1, 2, 3, 4, 5, 6, 12, 8, 9, 10, 11 };

    auto middleIter = test.begin() + (test.size() / 2);
    nth_element(test.begin(), middleIter, test.end());
    cout << "The median is: " << test[test.size() / 2] << endl;*/

    //colorize mesh

    //somehow give it to the window to render
    
    //Render the window
    return ShowWindow();
}
#endif