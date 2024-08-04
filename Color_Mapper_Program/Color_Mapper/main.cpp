#include <iostream>
#include <array>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <iomanip>
//#include "Delaunay_Triangulation.h"
#include "Hashtable.h"


using namespace std;

/*
void selectRandomRGB(){
    static default_random_engine  rng;
    uniform_int_distribution<int> distr(0,255);
}
*/

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
    for (int i = 0; i < 10; i++) {
        srand((unsigned) time(0));
        int result = 0 + (rand() % palette.size());
        cout << result << endl;
    }

/*
    //RUN DATA
    MeshData mesh;
    mesh = GenerateRandomMesh();

    //colorize mesh
    vector<double> cordinates;

    for (int x = 0; x < mesh.vertices.size(); x += 1) {
        cordinates.push_back(mesh.vertices[x].coordinates[0]);
        cordinates.push_back(mesh.vertices[x].coordinates[1]);
    }

    delaunator::Delaunator del(cordinates);
    for (int tri = 0; tri < del.triangles.size(); tri += 3)
    {
        Triangle triangle;
        triangle.vertices[0] = del.triangles[tri];
        triangle.vertices[1] = del.triangles[tri + 1];
        triangle.vertices[2] = (del.triangles[tri + 2]);

        //array<int, 3> randomColor = { ((float)rand() / RAND_MAX) * 255, ((float)rand() / RAND_MAX) * 255, ((float)rand() / RAND_MAX) * 255 };
        triangle.color = randomColor;

        mesh.triangles.push_back(triangle);
    }

    printMesh(mesh);

    HashTable HTable(mesh.vertices, mesh.triangles, palette);
    //Pass as pointer maybe

    //Check for completely random outputs
    bool colorSetSuccessful;
    for(int i = 0; i < HTable.getTriangleList().size(); i++){
        colorSetSuccessful = false;
        while(!colorSetSuccessful) {
            //Have Random here
            mesh.triangles[i].alterRBG(palette[0]);
            //Have the same color value for the chart
            HTable.insertHash(mesh.triangles[i].RGBvalue, mesh.triangles[i]);
        }
    }
    */
    //All triangles calculated and table is ready.
    cout << "done" << endl;
}

