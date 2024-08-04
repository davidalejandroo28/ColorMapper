#include <iostream>
#include <map>
#include <utility>
#include <random>
#include <list>

using namespace std;

#define DATASIZE 1000

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
    Triangle(){
        /*
        // "356-456-903"
        // 356 + (456 * 2) + (903 * 3)
        RGBvalue = color[0] + (color[1] * 2) + (color[2] * 3);
         */
    }
    bool checkNeighbors(Triangle newShape, array<Vertex,DATASIZE> vertex_location);
    void alterRBG(Color color);
};


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
    array<Vertex,DATASIZE> getVertexList();
    int hashFunction(int key);
    void insertHash(int key, Triangle &shape);
    void removeItem(int key);
    void printTable();
};