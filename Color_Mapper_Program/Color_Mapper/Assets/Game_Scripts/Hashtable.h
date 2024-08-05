#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <iomanip>

#include "Delaunay_Triangulation.h"

using namespace std;

//Ask about not even vertices count
//Ask about screen size

struct Color{
    array<int,3> RBG;
    Color(int in_R, int in_G, int in_B){
        RBG[0] = in_R;
        RBG[1] = in_G;
        RBG[2] = in_B;
    }
};

struct HashTriangle: Triangle
{
    int RGBvalue;

    bool checkNeighbors(HashTriangle newShape, array<Vertex, DATASIZE> vertex_location);
    void alterRBG(Color color);
};

class HashTable{
private:
    //buckets
    int hashGroups;
    //list<pair<int,vector<Triangle>>> *table;
    vector<list<HashTriangle>> table;
    array<Vertex,DATASIZE> vertex_list;
    vector<HashTriangle> triangles;
    vector<Color> color_list;

public:
    HashTable(array<Vertex,DATASIZE> new_list,vector<HashTriangle> new_triangles,vector<Color> colors){
        hashGroups = colors.size();
        table.resize(colors.size());
        vertex_list = new_list;
        triangles = new_triangles;
        color_list = colors;


    };
    vector<HashTriangle> getTriangleList();
    //array<Vertex,DATASIZE> getVertexList();
    int hashFunction(int key);
    void insertHash(HashTriangle &shape, Color in_color, bool &complete);
    void removeItem(HashTriangle key);
    void printTable();
};

//MeshData GenerateRandomMesh();
//int ReturnMidIndex(int lowIndex, int highIndex);
//int FindMedian(const array<Vertex, DATASIZE>& vertices, int lowIndex, int highIndex, bool axis);
//void PutPivotCenter(MeshData& mesh, const int lowIndex, const int midIndex, const int highIndex, const bool axis);
//void SortVertexs(MeshData& mesh, const int depth, const int lowIndex, const int highIndex);
//void printMesh(MeshData& mesh);
//void MergeMesh(MeshData& mesh, int lowVertIndex, int highVertIndex, int depth);