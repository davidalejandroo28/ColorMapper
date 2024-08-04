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
*/


struct MeshData
{
    array<Vertex, DATASIZE> vertices;
    vector<Triangle> triangles;

    array<float, 2> xAxisRange = { -10, 10 };
    array<float, 2> yAxisRange = { -10, 10 };
};


MeshData GenerateRandomMesh();
int ReturnMidIndex(int lowIndex, int highIndex);
int FindMedian(const array<Vertex, DATASIZE>& vertices, int lowIndex, int highIndex, bool axis);
void PutPivotCenter(MeshData& mesh, const int lowIndex, const int midIndex, const int highIndex, const bool axis);
void SortVertexs(MeshData& mesh, const int depth, const int lowIndex, const int highIndex);
void printMesh(MeshData& mesh);
void MergeMesh(MeshData& mesh, int lowVertIndex, int highVertIndex, int depth);