////CONFINGURATIONS////
//To configure for visual studio, go to project->properties->preprocessor->preprocessor definitions->set 1 or 0's accordingly

#if TESTMODE 1
    
#else
//don't run this main if tests are enabled

    #if DEBUGMODE 1
        #include <vld.h>
    #endif

#pragma once

#define DATASIZE 9

#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>

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

int ReturnMidIndex(int lowIndex, int highIndex)
{
    return (highIndex - lowIndex) / 2 + lowIndex;
}

//returns the index of the median in the array
int FindMedian(const array<Vertex, DATASIZE>& vertices, int lowIndex, int highIndex, bool axis)
{
    vector<float> medianArry;
    unordered_map<float, int> cordsToIndex;

    for (int i = lowIndex; i <= highIndex; i++)
    {
        medianArry.push_back(vertices.at(i).cordinates[axis]);
        cordsToIndex[vertices.at(i).cordinates[axis]] = i;
    }

    sort(medianArry.begin(), medianArry.end());

    return cordsToIndex.at(medianArry.at(ReturnMidIndex(0, medianArry.size() - 1)));
}

//puts the vertex pivot in the center based on the value of the cordinates (when axis = 0 (use x-axis), axis = 1 (use y-axis)
void PutPivotCenter(array<Vertex, DATASIZE>& vertices, const int lowIndex, const int midIndex, const int highIndex, const bool axis)
{
    int median = FindMedian(vertices, lowIndex, highIndex, axis);
    Vertex tempVertex = vertices.at(midIndex);
    vertices.at(midIndex) = vertices.at(median);
    vertices.at(median) = tempVertex;

    cout << "median is: " << " Axis " << axis << " " << vertices.at(midIndex).cordinates[axis] << endl;
}

//This sorts the vertex list into a 1D array kd-tree structure
//This uses quick sort
void SortVertexs(array<Vertex, DATASIZE>& vertices, const int depth = 0, const int lowIndex = 0, const int highIndex = DATASIZE - 1)
{
    //Return case to stop sorting
    if (lowIndex >= highIndex)
    {
        return;
    }

    //find middle point (it's m in:https://www.desmos.com/calculator/sbviponlyb)
    int midIndex = ReturnMidIndex(lowIndex, highIndex);
    bool useAxis = depth % 2; //do I use the x or y axis?

    PutPivotCenter(vertices, lowIndex, midIndex, highIndex, useAxis);
    
    //perform quicksort
    int leftIndex = lowIndex;
    int rightIndex = highIndex;

    //Have all lower vertices than the middle vertex be on the left from the middle vertex
    while (true)
    {
        if (leftIndex >= midIndex || rightIndex <= midIndex)
        {
            //all left elements are lower than the mid index
            break;
        }
        else if (vertices.at(leftIndex).cordinates[useAxis] < vertices.at(midIndex).cordinates[useAxis])
        {
            //this vertex is less than the middle
            leftIndex++;
        }
        else if (vertices.at(rightIndex).cordinates[useAxis] > vertices.at(midIndex).cordinates[useAxis])
        {
            //this vertex is higher than the middle
            rightIndex--;
        }
        else
        {
            //swap vertexs
            Vertex tempVert = vertices.at(leftIndex);
            vertices.at(leftIndex) = vertices.at(rightIndex);
            vertices.at(rightIndex) = tempVert;

            leftIndex++;
            rightIndex--;
        }
    }

    //recursivly call itself
    SortVertexs(vertices, depth + 1, lowIndex, midIndex - 1);
    SortVertexs(vertices, depth + 1, midIndex + 1, highIndex);
}

void printMesh(MeshData& mesh)
{
    cout << setprecision(2);

    cout << "----Printing vertices----" << endl;
    int index = 0;
    for (auto vertex = mesh.vertices.begin(); vertex != mesh.vertices.end(); vertex++)
    {
        cout << "Index: " << setw(3) << left << index << " ";
        cout << "X:" << setw(5) << left << vertex->cordinates[0] << 
               " Y:" << setw(5) << left << vertex->cordinates[1] << 
               " Tri Index: " << vertex->triangleIndex << endl;
        index++;
    }
    cout << "-------------------------" << endl;

    cout << "----Printing Triangles----" << endl;
    cout << "TODO" << endl;
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
    for (Vertex& vertex : mesh.vertices)
    {
        float randomVal = (float) rand() / RAND_MAX;

        //implementation of equation x or y = (distance of allowed bounds * random Value) + (left bound)
        //Proof: https://www.desmos.com/calculator/oat3knijz3
        vertex.cordinates[0] = ((xCordBounds[1] - xCordBounds[0]) * randomVal) + xCordBounds[0];
        vertex.cordinates[1] = ((yCordBounds[1] - yCordBounds[0]) * randomVal) + yCordBounds[0];
    }

    //Sort the mesh
    SortVertexs(mesh.vertices);



    return mesh;
}

int main()
{
    //Get user Input

    //generate a random mesh to colorize
    MeshData mesh = GenerateRandomMesh();
    printMesh(mesh);

    //colorize mesh

    //somehow give it to the window to render
    
    //Render the window
    return ShowWindow();
}
#endif