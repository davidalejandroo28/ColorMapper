#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <iomanip>
#include <array>

#include "Delaunay_Triangulation.h"

using namespace std;

class HashTable{
private:
    //buckets
    int hashGroups = 13;
    vector<list<Triangle*>> table;
    vector<Color> color_list;

    int hashFunction(array<int, 3> color);
public:
    HashTable(vector<Color> colors){
        table.resize(13);
        color_list = colors;
    };

    void insertHash(Triangle* shape, Color in_color, bool &complete);
};

void alterRGB(Triangle &new_t,Color new_color){
    new_t.color[0] = new_color.RGB[0];
    new_t.color[1] = new_color.RGB[1];
    new_t.color[2] = new_color.RGB[2];
}

bool isNeighbor(const Triangle* hashTriangle, const Triangle* shape)
{
    int count = 0;
    for (int v = 0; v < 3; v++)
    {
        for (int other = 0; other < 3; other++)
        {
            if (hashTriangle->vertices.at(v) == shape->vertices.at(other))
            {
                count++;
            }
        }
        
    }

    if (count >= 2)
    {
        return true;
    }

    return false;
}

int HashTable::hashFunction(array<int, 3> color) {
    int key = color[0] + (color[1] * 2) + (color[2] * 3);

    //Cyan,Magenta,BabyBlue
    if(to_string(key).length() == 4){
        //Baby Blue
        if(to_string(key)[to_string(key).size()-1] == '1'){
            return 7;
        }
        else{
            //Cyan
            if(to_string(key)[1] == '2'){
                return 4;
            }
            //Magenta
            else{
                return 5;
            }}}
    //Red,Blue,Green,Yellow,Orange,Indigo,Violet,Pink,Rose,Sand
    else if(to_string(key).length() == 3){
        //Red,Blue,Indigo,Orange
        if(key < 600){
            //Red
            if(key == 255){
                return 0;
            }
            //Blue
            else if(key == 510){
                return 1;
            }
            else{
                //Indigo
                if(key < 500){
                    return 8;
                }
                //Orange
                else{
                    return 6;
                }}}
        //Green,Yellow ,Violet,Pink,Rose,Sand
        else if(key > 600){
            //Green,Yellow,Violet
            if(key > 700 ){
                //Green
                if((key /3) == 255){
                    return 2;
                }
                //Yellow
                if(to_string(key)[1] == '5') {
                    return 3;
                }
                //Violet
                else{
                    return 9;
}}
            //Pink,Rose,Sand
            else if(key < 700){
                //Pink,Rose
                if(key < 650){
                    if(to_string(key)[2] == '8'){
                        return 10;
                    }
                    else{
                        return 11;
                    }}
                //Sand
                else{
                    return 12;
                }}}}
}

//Check if neighbors, and if so change color until colors are alright
void HashTable::insertHash(Triangle* shape, Color try_color, bool& complete) {
    int hashvalue = hashFunction(shape->color);

    list<Triangle*>& colorRow = table[hashvalue];

    if(colorRow.size() == 0) {
        colorRow.push_back(shape);
        complete = true;
    }
    else{  
        for (auto iter = begin(colorRow); iter != end(colorRow); iter++) {
            Triangle* hashTriangle = *iter;

            if (isNeighbor(hashTriangle, shape)) {
                //these triangles have the same color!
                complete = false;
                return;
            }
        }

        colorRow.push_back(shape);
        complete = true;
    }
}

HashTable* HTable = nullptr;
int triIndex = 0;
void ColorTriangle(MeshData& mesh, vector<Color> availableColors)
{
    if (triIndex >= mesh.triangles.size())
    {
        return;
    }

    int color_num = 0;
    bool colorSetHash = false;

    while (!colorSetHash) {
        color_num = (rand() % availableColors.size());
        alterRGB(mesh.triangles[triIndex], availableColors[color_num]);
        HTable->insertHash(&mesh.triangles[triIndex], availableColors[color_num], colorSetHash);
    }
}

bool ColorizeHTable(MeshData &mesh, vector<Color> availableColors, bool doInRunTime = false){
    srand(time(0));
    if(availableColors.size() < 4){
        return false;
    }

    if (HTable == nullptr)
        HTable = new HashTable(availableColors);

    if (doInRunTime == false)
    {
        for (int tri = 0; tri <= mesh.triangles.size(); tri++)
        {
            ColorTriangle(mesh, availableColors);
            triIndex++;
        }
    }
    else
    {
        ColorTriangle(mesh, availableColors);
        triIndex++;
    }
   
    return true;
}