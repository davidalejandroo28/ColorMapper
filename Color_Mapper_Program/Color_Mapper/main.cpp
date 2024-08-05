#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include "Hashtable.h"
//#include "Graph.h"
#include "Delaunay_Triangulation.h"


using namespace std;

void assignPalette(vector<Color>&palette, int option){
    palette.clear();
    if(option >= 1){
        Color Red(255,0,0);
        palette.push_back(Red);
        Color Orange(255,127,0);
        palette.push_back(Orange);
        Color Yellow(255,255,0);
        palette.push_back(Yellow);
        Color Green(0,255,0);
        palette.push_back(Green);
    }
    if(option >= 2){
        Color Blue(0,0,255);
        palette.push_back(Blue);
        Color BabyBlue(137,207,240);
        palette.push_back(BabyBlue);
        Color Indigo(75,0,130);
        palette.push_back(Indigo);
        Color Violet(148,0,211);
        palette.push_back(Violet);
    }
    if(option == 3){
        Color Lime(128,255,0);
        palette.push_back(Lime);
        Color Pink(255,0,127);
        palette.push_back(Pink);
        Color Rose(235,52,100);
        palette.push_back(Rose);
        Color Sand(173,145,66);
        palette.push_back(Sand);
    }
}




int main() {
    srand(time(0));

    int option;
    vector<Color> palette;
    bool done = false;
    while (!done) {
        cout << "Project 3: Mapping with Colors" << endl;
        cout << "Menu Options:" << endl;
        cout << "1. Small Color Palette (4 Colors)" << endl;
        cout << "2. Medium Color Palette (8 Colors)" << endl;
        cout << "3. Large Color Palette (12 Colors)" << endl;
        //cout << "2. Personal Color Palette" << endl;
        cout << "4. Color Mapping" << endl;
        cout << "Selection: ";
        cin >> option;
        cout << endl;
        if (option == 1 || option == 2 || option == 3) {
            assignPalette(palette, option);
        } else if (option == 4) {
            if (palette.size() < 4) {
                cout << "Please choose a color palette." << endl;
                cout << endl;
            } else {
                done = true;
            }
        }
    }

    //GENERATE MESH
    MeshData meshHash;
    meshHash = GenerateRandomMesh();
    vector<double> cordinates;
    for (int x = 0; x < meshHash.vertices.size(); x += 1)
    {
        cordinates.push_back(meshHash.vertices[x].coordinates[0]);
        cordinates.push_back(meshHash.vertices[x].coordinates[1]);
    }
    //GENERATE HASHTABLE
    //cout << meshHash.vertices.size() << endl;
    //cout << meshHash.triangles.size() << endl;

    HashTable HTable(meshHash.vertices, meshHash.triangles, palette);
    cout << "hi" << endl;
    //Graph graph;
    //graph.setNeighbors(mesh.triangles);

    //MeshData meshGraph = meshHash;
    //START COLOR ASSIGNING
    bool colorSetHash = false;
    bool colorSetGraph = false;
    int pal_num;
    /*
    cout << meshHash.triangles.size() << endl;
    for(int i = 0; i < meshHash.triangles.size(); i++){
        cout << meshHash.triangles[i].color[0] << endl;
        cout << meshHash.triangles[i].color[1] << endl;
        cout << meshHash.triangles[i].color[2] << endl;
    }
*/
    //cout << HTable.getTriangleList().size() << endl;
    //cout << HTable.getVertexList().size() << endl;
    for(int i = 0; i < HTable.getTriangleList().size(); i++){
        colorSetHash = false;
        colorSetGraph = false;
        while(!colorSetHash && !colorSetGraph) {
            pal_num = (rand()%palette.size());
            alterRBG(meshHash.triangles[i],palette[pal_num]);
            //Have the same color value for the chart
            HTable.insertHash(meshHash.triangles[i], palette[pal_num], colorSetHash);
            //graph.coloringShapes(mesh.triangles[i], colorSetGraph, palette[pal_num]);
        }
    }

    for(int i = 0; i < meshHash.triangles.size(); i++){
        cout << endl;
        cout << meshHash.triangles[i].color[0] << endl;
        cout << meshHash.triangles[i].color[1] << endl;
        cout << meshHash.triangles[i].color[2] << endl;
    }

    //HTable.printTable();

    //All triangles have colors
    cout << "done" << endl;
}

