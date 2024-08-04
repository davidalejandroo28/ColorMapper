#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <random>
#include <list>
#include "Hashtable.h"

using namespace std;



void Triangle::alterRBG(Color new_color){
    color[0] = new_color.RBG[0];
    color[1] = new_color.RBG[1];
    color[2] = new_color.RBG[2];
    RGBvalue = color[0] + (color[1] * 2) + (color[2] * 3);
}

bool Triangle::checkNeighbors(Triangle newShape, array<Vertex,DATASIZE> vertex_location) {
    for(int i = 0; i < sizeof(newShape.vertices); i ++){
        for(int j = 0; j < sizeof(newShape.vertices); j++) {
            if (vertex_location[vertices[i]].coordinates == vertex_location[newShape.vertices[j]].coordinates){
                return true;
            }
        }
    }
    return false;
}

int HashTable::hashFunction(int key) {
    return key % 10;
}

vector<Triangle> HashTable::getTriangleList() {
    return triangles;
}

//Check if neighbors, and if so change color until colors are alright
void HashTable::insertHash(int key, Triangle &shape, Color try_color) {
    int hashvalue = hashFunction(key);
    list<pair<int, vector<Triangle>>> &index = table[hashvalue];
    auto iter = begin(index);
    bool existing = false;
    bool sameColors = true;
    for (; iter != end(index); iter++) {
        if (iter->first == key) {
            for (int j = 0; j < iter->second.size(); j++) {
                if(iter->second[j].checkNeighbors(shape, vertex_list)){
                    while (sameColors) {
                        shape.alterRBG(color_list[0]);
                        if(shape.color[0] != iter->second[j].color[0] || shape.color[1] != iter->second[j].color[1] || shape.color[2] != iter->second[j].color[2]){
                            sameColors = false;
                        }
                    }
                    insertHash(shape.RGBvalue, shape, try_color);
                    break;
                }
            }
        }
        else{
            vector<Triangle> temp = {shape};
            index.emplace_back(key, temp);
            break;
        }
    }
}


void HashTable::removeItem(int key) {
    int hashvalue = hashFunction(key);
    list<pair<int,vector<Triangle>>> &index = table[hashvalue];
    auto iter = begin(index);
    for(; iter != end(index); iter ++){
        if(iter->first == key){
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
                cout << "Key: " << iter->first << endl;
                for(int i = 0; i < iter->second.size(); i++) {
                    cout << "Value(s): " << iter->second[i].RGBvalue << ' ';
                }
                cout << endl;
            }
        }
    }
}