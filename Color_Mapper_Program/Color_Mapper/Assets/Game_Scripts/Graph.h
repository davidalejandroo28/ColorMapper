//
// Created by DavidD on 7/29/2024.
//
#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <utility>
#include <random>
#include <map>
#include <queue>

#include "Delaunay_Triangulation.h"

using namespace std;

class Graph {
    bool realTime = false;
    int triIndex = -1;
public:
    void disableRealTime()
    {
        this->realTime = false;
        triIndex = -1;
    }

    void enableRealTime()
    {
        this->realTime = true;
        triIndex = 0;
    }

    bool doRealTime()
    {
        return realTime;
    }

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

    //returns if it's successful 
    bool colorMesh(MeshData& meshData, vector<Color>& availableColors)
    {        
        if (realTime)
        {
            if (triIndex >= meshData.triangles.size())
            {
                return true;
            }

            queue<Color> usableColors;

            for (int color = 0; color < availableColors.size(); color++)
            {
                bool usedColor = false;

                for (int x = 0; x < meshData.triangles[triIndex].neighbors.size(); x++)
                {
                    if (availableColors.at(color) == meshData.triangles[triIndex].neighbors[x])
                    {
                        usedColor = true;
                        break;
                    }
                }

                if (usedColor == false)
                {
                    usableColors.push(availableColors.at(color));
                }

            }

            if (usableColors.size() > 0)
            {
                meshData.triangles.at(triIndex).color = usableColors.front().RGB;
            }
            else
            {
                return false;
            }

            triIndex++;
            return true;
        }
        else
        {
            for (int tri = 0; tri < meshData.triangles.size(); tri++)
            {
                queue<Color> usableColors;

                for (int color = 0; color < availableColors.size(); color++)
                {
                    bool usedColor = false;

                    for (int x = 0; x < meshData.triangles[tri].neighbors.size(); x++)
                    {
                        if (availableColors.at(color) == meshData.triangles[tri].neighbors[x])
                        {
                            usedColor = true;
                            break;
                        }
                    }

                    if (usedColor == false)
                    {
                        usableColors.push(availableColors.at(color));
                    }
                }

                if (usableColors.size() > 0)
                {
                    meshData.triangles.at(tri).color = usableColors.front().RGB;
                }
                else
                {
                    return false;
                }
            }
        } 
    }

    void coloringShapes(Triangle& T, bool& check, Color color_par){

        // check if neighbors have color_par
        if(T.color.empty()){
            T.color[0] = color_par.RGB[0];
            T.color[1] = color_par.RGB[1];
            T.color[2] = color_par.RGB[2];
        }
        
        for(int i = 0; i < T.neighbors.size(); i++){
            int count = 0; 
            if(T.neighbors[i]->color[0] == T.color[0]){
                count++;
            }
            if(T.neighbors[i]->color[1] == T.color[1]){
                count++;
            }
            if(T.neighbors[i]->color[2] == T.color[2]){
                count++;
            }
            if(count == 3){
                return;
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













