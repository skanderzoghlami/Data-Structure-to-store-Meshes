#include <iostream >
#include <vector>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <utility>

using std::cout ; 
using std::endl ;
using std::vector ;
using namespace std;


struct Vertex {
    float x, y , z ;
    int front_index  ;
} ;
struct Face{
    int v1,v2,v3 ;
    int f1,f2,f3 ; 
};

void readOFFFile( const string& filename, std::vector<Vertex>& vertices, std::vector<Face>& faces){
    std::ifstream file(filename);
    std::string line;

     int numVertices, numFaces, numEdges;
     string holder;
     file>> holder;
     file >> numVertices >> numFaces >> numEdges;
     vertices.resize(numVertices);
     faces.resize(numFaces);

     map<pair<int,int>,pair<int,int>> indexing_map ;
     
   for (int i = 0; i < numVertices; ++i) {
        Vertex vertex;
        file >> vertex.x >> vertex.y >> vertex.z;
        vertex.front_index = -1 ;
        vertices[i] = vertex ;
    }

    for (int i = 0; i < numFaces; ++i) {
        Face face;

        face.f1 = -1 ;
        face.f2 = -1 ;
        face.f3 = -1 ;
        
        int numVerticesInFace;
        file >> numVerticesInFace >> face.v1 >> face.v2 >> face.v3;
        faces[i]=face ;

        std::map<std::pair<int, int>, std::pair<int, int>>::iterator iter = indexing_map.find({face.v1 , face.v2 }) ;
        if (iter != indexing_map.end()) {
            int otherFace = iter->second.first;  
            int otherVertex = iter->second.second; 
            vertices[otherVertex].front_index = i ;
        if (faces[otherFace].f1 == -1 ) {
                faces[otherFace].f1 = i ;
         } else if (faces[otherFace].f2 ==-1) {
                faces[otherFace].f2 = i ;
         } else if (faces[otherFace].f3 ==-1) {
                faces[otherFace].f3 = i ;
        }
        } else{
            indexing_map[{face.v1 , face.v2 }] = { i , face.v3}; // cette aréte appartient au face i et la sommet qu'il faut mettre est v3
        }

        iter = indexing_map.find({face.v1 , face.v3 }) ;
        if (iter != indexing_map.end()) {
             int otherFace = iter->second.first;  
            int otherVertex = iter->second.second; 
            vertices[otherVertex].front_index = i ;
        if (faces[otherFace].f1 == -1 ) {
                faces[otherFace].f1 = i ;
         } else if (faces[otherFace].f2 ==-1) {
                faces[otherFace].f2 = i ;
         } else if (faces[otherFace].f3 ==-1) {
                faces[otherFace].f3 = i ;
        }
        } else{
            indexing_map[{face.v1 , face.v3 }] = { i , face.v2};
        }

        iter = indexing_map.find({face.v2 , face.v3 }) ;
        if (iter != indexing_map.end()) {
            int otherFace = iter->second.first;  
            int otherVertex = iter->second.second; 
            vertices[otherVertex].front_index = i ;
        if (faces[otherFace].f1 == -1 ) {
                faces[otherFace].f1 = i ;
         } else if (faces[otherFace].f2 ==-1) {
                faces[otherFace].f2 = i ;
         } else if (faces[otherFace].f3 ==-1) {
                faces[otherFace].f3 = i ;
        }
        } else{
            indexing_map[{face.v2 , face.v3 }] = { i , face.v1};
        }
    }
 file.close();
}



void saveOFFFile(const vector<Vertex>& vertices, const vector<Face>& faces, const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << " for writing." << endl;
        return;
    }

    outFile << "OFF" << endl;
    outFile << vertices.size() << " " << faces.size() << " 0" << endl;
    for (const Vertex& vertex : vertices) {
        outFile << vertex.x << " " << vertex.y << " " << vertex.z << endl;
    }
    for (const Face& face : faces) {
        outFile << "3 " << face.v1 << " " << face.v2 << " " << face.v3 << endl;
    }
    outFile.close();
}

void CreatePyramid(){
    vector<Vertex> Vertices ; 
vector<Face> Faces ; 

    Vertex v0 = {0.0f, 0.0f, 0.0f, -1};
    Vertex v1 = {1.0f, 0.0f, 0.0f, -1};
    Vertex v2 = {1.0f, 1.0f, 0.0f, -1};
    Vertex v3 = {0.0f, 1.0f, 0.0f, -1};
    Vertex v4 = {0.5f, 0.5f, 1.0f, -1}; 

    Vertices.push_back(v0);
    Vertices.push_back(v1);
    Vertices.push_back(v2);
    Vertices.push_back(v3);
    Vertices.push_back(v4);

    Face f0 = {0, 1, 4, -1, 1, 2};
    Face f1 = {1, 2, 4, 0, -1, 3};
    Face f2 = {2, 3, 4, 1, -1, 4};
    Face f3 = {3, 0, 4, 2, -1, 0};
    Face f4 = {0, 1, 2, 3, 4, -1}; 

    Faces.push_back(f0);
    Faces.push_back(f1);
    Faces.push_back(f2);
    Faces.push_back(f3);
    Faces.push_back(f4);
    saveOFFFile(Vertices, Faces, "pyramid.off");
}

int main(){
 // CreatePyramid() ;
    std::vector<Face> faces(5) ;
    std::vector<Vertex> vertices(5) ;
    readOFFFile("pyramid.off",vertices , faces);
   
    std::cout << "Vertices:" << std::endl;
    for (const Vertex& vertex : vertices) {
        std::cout << "x: " << vertex.x << ", y: " << vertex.y << ", z: " << vertex.z << ", face equivalent: " << vertex.front_index << std::endl;
    }

    std::cout << "Faces:" << std::endl;
    for (const Face& face : faces) {
    std::cout << "vertices: " << face.v1 << ", " << face.v2 << ",  " << face.v3 << ", faces : " << face.f1 << ",    " << face.f2 << ",   " << face.f3 << std::endl;
    }


return 0 ; 
}

// working with maps :
//     indexing_map[{1, 2}] = {3, 4};
//     indexing_map[{5, 6}] = {7, 8};
//     indexing_map[{9, 10}] = {11, 12};

    // Access and print values from the map
//     std::pair<int, int> key = {1, 2};
//     std::map<std::pair<int, int>, std::pair<int, int>>::iterator it = indexing_map.find(key);

//     if (it != indexing_map.end()) {
 //        std::pair<int, int> value = it->second;
 //        std::cout << "Key: (" << key.first << ", " << key.second << "), Value: (" << value.first << ", " << value.second << ")\n";
 //    } else {
  //       std::cout << "Key not found in the map.\n";
  //   }