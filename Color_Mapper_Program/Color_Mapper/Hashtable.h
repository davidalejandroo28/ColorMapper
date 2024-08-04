#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <iomanip>
#include "Hashtable.h"

#define DATASIZE 1000

using namespace std;

struct Color{
    array<int,3> RBG;
    Color(int in_R, int in_G, int in_B){
        RBG[0] = in_R;
        RBG[1] = in_G;
        RBG[2] = in_B;
    }
};

struct Vertex
{
    //X cordinate is 0, Y cordinate is 1
    array<float, 2> coordinates;
    int triangleIndex;
};

struct Triangle{
    int RGBvalue;
    array<int,3> color;
    //pos of vertices in vertex array
    array<int,3> vertices;
    Triangle(){}
    bool checkNeighbors(Triangle newShape, array<Vertex,DATASIZE> vertex_location);
    void alterRBG(Color color);
};


class HashTable{
private:
    //buckets
    int hashGroups;
    list<pair<int,vector<Triangle>>> *table;
    array<Vertex,DATASIZE> vertex_list;
    vector<Triangle> triangles;
    vector<Color> color_list;
public:
    HashTable(array<Vertex,DATASIZE> new_list,vector<Triangle> new_triangles,vector<Color> colors){
        hashGroups = colors.size();
        table = new list<pair<int,vector<Triangle>>>[hashGroups];
        vertex_list = new_list;
        triangles = new_triangles;
        color_list = colors;


    };
    vector<Triangle> getTriangleList();
    array<Vertex,DATASIZE> getVertexList();
    int hashFunction(int key);
    void insertHash(int key, Triangle &shape, Color in_color);
    void removeItem(int key);
    void printTable();
};




/*







struct Triangle
{
    array<int, 3> triNeigh;
    array<int, 3> vertices; //<== points to each vertex index in the vertice array
    array<int, 3> color = { 255, 255, 255 }; //color range is 0 to 255 and R G B
};



struct MeshData
{
    array<Vertex, DATASIZE> vertices;
    vector<Triangle> triangles;

    array<float, 2> xAxisRange = { -10, 10 };
    array<float, 2> yAxisRange = { -10, 10 };
};


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
        medianArry.push_back(vertices.at(i).coordinates[axis]);
        cordsToIndex[vertices.at(i).coordinates[axis]] = i;
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

    cout << "median is: " << " Axis " << axis << " " << mesh.vertices.at(midIndex).coordinates[axis] << endl;
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
        else if (mesh.vertices.at(leftIndex).coordinates[useAxis] < mesh.vertices.at(midIndex).coordinates[useAxis])
        {
            //this vertex is less than the middle
            leftIndex++;
        }
        else if (mesh.vertices.at(rightIndex).coordinates[useAxis] > mesh.vertices.at(midIndex).coordinates[useAxis])
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
        cout << "X:" << setw(5) << left << vertex->coordinates[0] <<
             " Y:" << setw(5) << left << vertex->coordinates[1] <<
             " Tri Index: " << vertex->triangleIndex << endl;
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
        //array<int, 3> randomColor = { ((float)rand() / RAND_MAX) * 255, ((float)rand() / RAND_MAX) * 255, ((float)rand() / RAND_MAX) * 255 };
        mesh.vertices.at(lowVertIndex).color = randomColor;
        mesh.vertices.at(lowVertIndex + 1).color = randomColor;
        mesh.vertices.at(lowVertIndex + 2).color = randomColor;

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

//Generate random data set
MeshData GenerateRandomMesh()
{
    //make a seed (this should somehow be randomized)
    std::srand(1);

    MeshData mesh;
    float randomVal = 0;

    //place each vertex randomly on a 2d plane
    for (Vertex& vertex : mesh.vertices)
    {
        randomVal = (float)rand() / RAND_MAX;

        //implementation of equation x or y = (distance of allowed bounds * random Value) + (left bound)
        //Proof: https://www.desmos.com/calculator/oat3knijz3
        vertex.coordinates[0] = ((mesh.xAxisRange[1] - mesh.xAxisRange[0]) * randomVal) + mesh.xAxisRange[0];

        randomVal = (float)rand() / RAND_MAX;
        vertex.coordinates[1] = ((mesh.yAxisRange[1] - mesh.yAxisRange[0]) * randomVal) + mesh.yAxisRange[0];
    }

    //setting camera bounds
    mesh.vertices[0].coordinates = { mesh.xAxisRange[0], mesh.yAxisRange[0] };
    mesh.vertices[1].coordinates = { mesh.xAxisRange[0], mesh.yAxisRange[1] };
    mesh.vertices[2].coordinates = { mesh.xAxisRange[1], mesh.yAxisRange[0] };
    mesh.vertices[3].coordinates = { mesh.xAxisRange[1], mesh.yAxisRange[1] };

    //Sort the mesh
    //SortVertexs(mesh);

    //merge the mesh together
    //MergeMesh(mesh);

    return mesh;
}
*/