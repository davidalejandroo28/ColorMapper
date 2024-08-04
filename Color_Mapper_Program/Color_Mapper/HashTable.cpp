#include <iostream>
#include <map>
#include <utility>
#include <random>
#include <list>
#include "Delaunay_Triangulation.h"
#include "Hashtable.h"

using namespace std;


int HashTable::hashFunction(int key) {
    return key % 10;
}

//Check if neighbors, and if so change color until colors are alright
void HashTable::insertHash(int key, Triangle &shape) {
    int hashvalue = hashFunction(key);
    list<pair<int, vector<Triangle>>> &index = table[hashvalue];
    auto iter = begin(index);
    bool existing = false;
    bool sameColors = true;
    for (; iter != end(index); iter++) {
        if (iter->first == key) {
            for (int j = 0; j < iter->second.size(); j++) {
                if(iter->second[j].checkNeighbors(shape)){
                    while (sameColors) {
                        shape.alterRBG(color_list);
                        if(shape.color[0] != iter->second[j].color[0] || shape.color[1] != iter->second[j].color[1] || shape.color[2] != iter->second[j].color[2]){
                            sameColors = false;
                        }
                    }
                    insertHash(shape.RGBvalue, shape);
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