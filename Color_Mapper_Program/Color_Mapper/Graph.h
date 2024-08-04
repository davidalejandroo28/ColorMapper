////
//// Created by DavidD on 7/29/2024.
////
//
//#ifndef PROJECT3_GRAPH_H
//#define PROJECT3_GRAPH_H
//#include <iostream>
//#include <algorithm>
//#include <vector>
//using namespace std;
//
//struct Shape{ //Modify structure where vector is array
//    vector<pair<float, float>> vertices;
//    vector<Shape*> neighbors;
//    int color;
//
//    Shape(vector<pair<int, int>> v, int c){
//        for(int i = 0; i < v.size(); i++){
//            vertices[i] = v[i];
//        }
//        color = c;
//    };
//
//};
//
//class Graph {
//public:
//    vector<Shape*> shapes;
//    vector<int> colors = {1, 2, 3, 4};
//
//    void setNeighbors(){
//        for(int i = 0; i < shapes.size(); i++){
//            for(int j = 0; j < shapes.size(); j++){
//                int common_vertices = 0;
//                if (find(shapes[i]->vertices.begin(), shapes[i]->vertices.end(),shapes[j]->vertices[0])!= shapes[i]->vertices.end() ){
//                    common_vertices++;
//                }
//                if (find(shapes[i]->vertices.begin(), shapes[i]->vertices.end(),shapes[j]->vertices[1])!= shapes[i]->vertices.end() ){
//                    common_vertices++;
//                }
//                if (find(shapes[i]->vertices.begin(), shapes[i]->vertices.end(),shapes[j]->vertices[2])!= shapes[i]->vertices.end() ){
//                    common_vertices++;
//                }
//                if(common_vertices == 2){
//                    shapes[i]->neighbors.push_back(shapes[j]);
//                }
//                else if(common_vertices == 3){
//                    continue;
//                }
//
//            }
//        }
//    }
//
//    void coloringShapes(){
//        for(int i = 0; i < shapes.size(); i++){
//            colors = {1, 2, 3, 4};
//            for(int j = 0; j < shapes[i]->neighbors.size(); j++){
//                int temp_col = shapes[i]->neighbors[j]->color;
//                auto it = find(colors.begin(), colors.end(), temp_col);
//                if (it != colors.end()) {
//                    colors.erase(it);
//                }
//            }
//            if(colors.size() != 0) {
//                shapes[i]->color = colors[0];
//            }
//        }
//    }
//
//};




//#endif //PROJECT3_GRAPH_H
