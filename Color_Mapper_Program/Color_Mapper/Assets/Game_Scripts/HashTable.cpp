#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <random>
#include <list>
#include <array>

#include "Hashtable.h"

using namespace std;

void HashTriangle::alterRBG(Color new_color){
    color[0] = new_color.RBG[0];
    color[1] = new_color.RBG[1];
    color[2] = new_color.RBG[2];
    RGBvalue = color[0] + (color[1] * 2) + (color[2] * 3);
}

bool HashTriangle::checkNeighbors(HashTriangle newShape, array<Vertex,DATASIZE> vertex_location) {
    int cnt_same_vert = 0;
    for(int i = 0; i < sizeof(newShape.vertices); i ++){
        for(int j = 0; j < sizeof(newShape.vertices); j++) {
            if (vertex_location[vertices[i]].cordinates == vertex_location[newShape.vertices[j]].cordinates){
                cnt_same_vert += 1;
                break;
            }
        }
        if(cnt_same_vert >= 2){
            return true;
        }
    }
    return false;
}

int HashTable::hashFunction(int key) {
    return key % 10;
}

vector<HashTriangle> HashTable::getTriangleList() {
    return triangles;
}

//Check if neighbors, and if so change color until colors are alright
void HashTable::insertHash(HashTriangle &shape, Color try_color, bool &complete) {
    int hashvalue = hashFunction(shape.RGBvalue);
    //list<pair<int, vector<Triangle>>> &index = table[hashvalue];
    list<HashTriangle> index = table[hashvalue];
    if(index.empty()) {
        index.push_back(shape);
        complete = true;
    }
    else{
        auto iter = begin(index);
        for (; iter != end(index); iter++) {
                if (iter->checkNeighbors(shape, vertex_list)) {
                    //for (int j = 0; j < iter->second.size(); j++) {
                    //if (iter->second[j].checkNeighbors(shape, vertex_list)) {
                    //break;
                    //}
                    //}
                    break;
                }
            }
        }
}


void HashTable::removeItem(HashTriangle shape) {
    int hashvalue = hashFunction(shape.RGBvalue);
    list<HashTriangle> index = table[hashvalue];
    auto iter = begin(index);
    for(; iter != end(index); iter ++){
        if(iter->vertices == shape.vertices){
            iter = index.erase(iter);
        }
    }
}

void HashTable::printTable() {
    for(int i = 0; i < hashGroups; i ++){
        if(table[i].size() == 0){
            continue;
        }
        else{
            auto iter = begin(table[i]);
            for(; iter != end(table[i]); iter ++){
                cout << "Bucket: " << i + 1 << endl;
                cout << "Key: " << iter->RGBvalue << endl;
                cout << "Value(s): ";
                for(int i = 0; i < iter->vertices.size(); i++) {
                    cout << iter->vertices[i] << ' ';
                }
                cout << endl;
            }
        }
    }
}

/*
//MESH

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
        //mesh.vertices.at(lowVertIndex).color = randomColor;
        //mesh.vertices.at(lowVertIndex + 1).color = randomColor;
        //mesh.vertices.at(lowVertIndex + 2).color = randomColor;

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