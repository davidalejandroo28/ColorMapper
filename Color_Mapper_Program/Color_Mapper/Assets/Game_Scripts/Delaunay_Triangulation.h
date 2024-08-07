#pragma once

#include <array>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <time.h>

#include "../../Libraries/delaunator.hpp"

#define DATASIZE 3000 // The algorithm is capable of proccessing a datasize of 100,000, but is 3000 on the executable so it is faster on the TAs end.

struct Vertex
{
    //X cordinate is 0, Y cordinate is 1
    array<float, 2> cordinates;
};

struct Triangle
{
    array<int, 3> vertices = {}; //<== points to each vertex index in the vertice array
    array<int, 3> color = { 60, 60, 60 }; //color range is 0 to 255 and R G B
    vector<Triangle*> neighbors; //For algorithm using Graph Adjacency list 
};

struct MeshData
{
    array<Vertex, DATASIZE>* verticesAlloc = new array<Vertex, DATASIZE>();
    array<Vertex, DATASIZE>& vertices = *verticesAlloc;
    vector<Triangle> triangles;

    array<float, 2> xAxisRange = { -10, 10 };
    array<float, 2> yAxisRange = { -10, 10 };

    void DeleteResources()
    {
        delete[] verticesAlloc;
    }
};

struct Color {
    array<int, 3> RGB;
    Color(int R, int G, int B) {
        RGB[0] = R;
        RGB[1] = G;
        RGB[2] = B;
    }

    bool operator==(Triangle* triangle)
    {
        if (triangle->color == this->RGB)
        {
            return true;
        }

        return false;
    }

    bool operator==(Color& color)
    {
        if (color.RGB == this->RGB)
        {
            return true;
        }

        return false;
    }
};

//Generate random data set
MeshData* GenerateRandomMesh()
{
    //make a seed (this is randomized by time)
    std::srand(time(0));

    MeshData* mesh = new MeshData();
    float randomVal = 0;

    //place each vertex randomly on a 2d plane
    for (Vertex& vertex : mesh->vertices)
    {
        randomVal = (float)rand() / RAND_MAX;

        //implementation of equation x or y = (distance of allowed bounds * random Value) + (left bound)
        //Proof: https://www.desmos.com/calculator/oat3knijz3
        vertex.cordinates[0] = ((mesh->xAxisRange[1] - mesh->xAxisRange[0]) * randomVal) + mesh->xAxisRange[0];

        randomVal = (float)rand() / RAND_MAX;
        vertex.cordinates[1] = ((mesh->yAxisRange[1] - mesh->yAxisRange[0]) * randomVal) + mesh->yAxisRange[0];
    }

    //setting camera bounds
    mesh->vertices[0].cordinates = { mesh->xAxisRange[0], mesh->yAxisRange[0] };
    mesh->vertices[1].cordinates = { mesh->xAxisRange[0], mesh->yAxisRange[1] };
    mesh->vertices[2].cordinates = { mesh->xAxisRange[1], mesh->yAxisRange[0] };
    mesh->vertices[3].cordinates = { mesh->xAxisRange[1], mesh->yAxisRange[1] };

    //Sort the mesh
    //SortVertexs(mesh);

    //merge the mesh together
    //MergeMesh(mesh);

    //Using a library to connect the points together b/c of time constraints
    vector<double> cordinates;

    for (int x = 0; x < mesh->vertices.size(); x += 1)
    {
        cordinates.push_back(mesh->vertices[x].cordinates[0]);
        cordinates.push_back(mesh->vertices[x].cordinates[1]);
    }

    delaunator::Delaunator del(cordinates);
    for (int tri = 0; tri < del.triangles.size(); tri += 3)
    {
        Triangle triangle;
        triangle.vertices[0] = del.triangles[tri];
        triangle.vertices[1] = del.triangles[tri + 1];
        triangle.vertices[2] = (del.triangles[tri + 2]);

        //array<int, 3> randomColor = { ((float)rand() / RAND_MAX) * 255, ((float)rand() / RAND_MAX) * 255, ((float)rand() / RAND_MAX) * 255 };
        //triangle.color = randomColor;

        mesh->triangles.push_back(triangle);
    }

    return mesh;
}

/*
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
    array<float, partitionSize> partitionData{};
    */

//-------Not USED! This is leftover from my attempt to implement the delaugnay triangulation algorithm (it took to long so I got a library instead-----
int ReturnMidIndex(int lowIndex, int highIndex)
{
    return (highIndex + lowIndex) / 2;
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
void PutPivotCenter(MeshData& mesh, const int lowIndex, const int midIndex, const int highIndex, const bool axis)
{
    int median = FindMedian(mesh.vertices, lowIndex, highIndex, axis);
    Vertex tempVertex = mesh.vertices.at(midIndex);
    mesh.vertices.at(midIndex) = mesh.vertices.at(median);
    mesh.vertices.at(median) = tempVertex;

    cout << "median is: " << " Axis " << axis << " " << mesh.vertices.at(midIndex).cordinates[axis] << endl;
}

//This sorts the vertex list into a 1D array kd-tree structure
//This uses quick sort
void SortVertexs(MeshData& mesh, const int depth = 0, const int lowIndex = 0, const int highIndex = DATASIZE - 1)
{
    //Return case to stop sorting
    if (lowIndex >= highIndex)
    {
        return;
    }

    //find middle point (it's m in:https://www.desmos.com/calculator/sbviponlyb)
    int midIndex = ReturnMidIndex(lowIndex, highIndex);
    bool useAxis = depth % 2; //do I use the x or y axis?

    PutPivotCenter(mesh, lowIndex, midIndex, highIndex, useAxis);

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
        else if (mesh.vertices.at(leftIndex).cordinates[useAxis] < mesh.vertices.at(midIndex).cordinates[useAxis])
        {
            //this vertex is less than the middle
            leftIndex++;
        }
        else if (mesh.vertices.at(rightIndex).cordinates[useAxis] > mesh.vertices.at(midIndex).cordinates[useAxis])
        {
            //this vertex is higher than the middle
            rightIndex--;
        }
        else
        {
            //swap vertexs
            Vertex tempVert = mesh.vertices.at(leftIndex);
            mesh.vertices.at(leftIndex) = mesh.vertices.at(rightIndex);
            mesh.vertices.at(rightIndex) = tempVert;

            leftIndex++;
            rightIndex--;
        }
    }

    //recursivly call itself
    SortVertexs(mesh, depth + 1, lowIndex, midIndex - 1);
    SortVertexs(mesh, depth + 1, midIndex + 1, highIndex);
}

void printMesh(MeshData& mesh)
{
    cout << setprecision(2);
    cout << setfill(' ');

    cout << "----Printing vertices----" << endl;
    int index = 0;
    for (auto vertex = mesh.vertices.begin(); vertex != mesh.vertices.end(); vertex++)
    {
        cout << "Index: " << setw(3) << left << index << " ";
        cout << "X:" << setw(5) << left << vertex->cordinates[0] <<
            " Y:" << setw(5) << left << vertex->cordinates[1] << endl;
        index++;
    }
    cout << "-------------------------" << endl << endl;

    cout << "----Printing Triangles----" << endl;
    index = 0;
    for (auto triangle = mesh.triangles.begin(); triangle != mesh.triangles.end(); triangle++)
    {
        cout << "Triangle: " << index <<
            " Vertices: " << triangle->vertices[0] << " " << triangle->vertices[1] << " " << triangle->vertices[2] << endl;
        index++;
    }
    cout << "-------------------------" << endl << endl;

    cout << "TODO, Triangle neighbors" << endl;
}

//using Delaunay Trianglization (A Divide-and-conquer Delaunay Triangulation by sang-wook yang, young choi, and chang-kyo jung)
void MergeMesh(MeshData& mesh, int lowVertIndex = 0, int highVertIndex = DATASIZE - 1, int depth = 0)
{
    if (highVertIndex - lowVertIndex == 2)
    {
        //connect the vertices into a triangle
        Triangle triangle;
        triangle.vertices = { lowVertIndex, lowVertIndex + 1, lowVertIndex + 2 };
        mesh.triangles.push_back(triangle);

        //randomize color to see different triangles
        array<int, 3> randomColor = { ((float)rand() / RAND_MAX) * 255, ((float)rand() / RAND_MAX) * 255, ((float)rand() / RAND_MAX) * 255 };
        /*mesh.vertices.at(lowVertIndex).color = randomColor;
        mesh.vertices.at(lowVertIndex + 1).color = randomColor;
        mesh.vertices.at(lowVertIndex + 2).color = randomColor;*/

        //TODO, add triangle indexs when needed
        return;
    }
    else if (highVertIndex - lowVertIndex < 2)
    {
        //leave the points be for the merge algorithm
        return;
    }

    int midIndex = ReturnMidIndex(lowVertIndex, highVertIndex);

    //merge stuff
    //let's do recursive call on merge mesh and get something on the screen before I do the merge function
    MergeMesh(mesh, lowVertIndex, midIndex);
    MergeMesh(mesh, midIndex + 1, highVertIndex);
}
