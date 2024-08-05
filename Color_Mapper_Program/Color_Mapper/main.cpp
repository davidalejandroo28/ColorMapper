#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include "Hashtable.h"
#include "Graph.h"


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

    //Test random
/*
    for (int i = 0; i < 100; i++) {
        cout << (rand()%10) << endl;
    }
*/

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

    MeshData mesh;
    mesh = GenerateRandomMesh();
    vector<double> cordinates;
    for (int x = 0; x < mesh.vertices.size(); x += 1)
    {
        cordinates.push_back(mesh.vertices[x].coordinates[0]);
        cordinates.push_back(mesh.vertices[x].coordinates[1]);
    }
    printMesh(mesh);

    //GENERATE HASHTABLE
    HashTable HTable(mesh.vertices, mesh.triangles, palette);

    //Generate adjacency list
    Graph graph;
    graph.setNeighbors(mesh.triangles);

/*
    //START COLOR ASSIGNING
    bool colorSetSuccessful;
    int pal_num;
    for(int i = 0; i < HTable.getTriangleList().size(); i++){
        colorSetSuccessful = false;
        while(!colorSetSuccessful) {
            cout << rand()%palette.size() << endl;
            pal_num = (rand()%palette.size());
            mesh.triangles[i].alterRBG(palette[pal_num]);
            //Have the same color value for the chart
            HTable.insertHash(mesh.triangles[i].RGBvalue, mesh.triangles[i], palette[pal_num]);
            
            graph.coloringShapes(mesh.triangles[i], colorSetSuccessful, palette[pal_num]); // Graph Algorithm
            
        }
    }
*/
    //All triangles have colors
    cout << "done" << endl;
}

