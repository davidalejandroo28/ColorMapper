//
// Created by DavidD on 7/29/2024.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <utility>
#include <random>
#include <map>
//#include "Delaunay_Triangulation.h"
using namespace std;

struct Color {
    array<int, 3> RGB;
    Color(int R, int G, int B){
        RGB[0] = R;
        RGB[1] = G;
        RGB[2] = B;
    }
};

class Graph {
public:
    void setNeighbors(vector<Triangle>& shapes) {
        int numTriangles = shapes.size();
        for (int i = 0; i < numTriangles; ++i) {
            for (int j = 0; j < numTriangles; ++j) {
                int commonVertices = 0;
                for (int k = 0; k < 3; ++k) {
                    if (find(begin(shapes[j].vertices), end(shapes[j].vertices), shapes[i].vertices[k]) != end(shapes[j].vertices)) {
                        ++commonVertices;
                    }
                }
                // If they share only 2 vertices, they are neighbors,
                if (commonVertices == 2) {
                    shapes[i].neighbors.push_back(&shapes[j]);
                }
            }
        }
    }

//while(!checker)
     void coloringShapes(Triangle& T, bool& check, Color color_par){

        // check if neighbors have color_par
        for(int i = 0; i < T.neighbors.size(); i++){
            if(T.neighbors[i]->color == T.color){
                return; // just return because a neighbor has the same color as Triangle parameter
            }
        }
        check = true; // if neighbors don't have color check = true
        for(int j = 0; j < 3; j++){
            T.color[0] = color_par.RGB[0];
            T.color[1] = color_par.RGB[1];
            T.color[2] = color_par.RGB[2];
        }
    }




};












