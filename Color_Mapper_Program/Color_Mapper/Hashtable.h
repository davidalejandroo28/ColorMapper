#include <iostream>
#include <map>
#include <utility>
#include <random>
#include <list>
#include "Delaunay_Triangulation.h"

using namespace std;



class HashTable{
private:
    static const int  hashGroups = 10;
    list<pair<int,vector<Triangle>>> table[hashGroups];
    array<Vertex,DATASIZE> vertex_list;
    vector<Triangle> triangles;
    vector<Color> color_list;
public:
    HashTable(array<Vertex,DATASIZE> new_list,vector<Triangle> new_triangles,vector<Color> colors){
        vertex_list = new_list;
        triangles = new_triangles;
        color_list = colors;


    };
    vector<Triangle> getTriangleList();
    int hashFunction(int key);
    void insertHash(int key, Triangle &shape);
    void removeItem(int key);
    void printTable();
};