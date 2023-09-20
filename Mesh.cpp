#include <iostream >
#include <vector>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include "Mesh.h"

using std::cout ; 
using std::endl ;
using std::vector ;
using namespace std;



Mesh::Mesh(const std::vector<Vertex>& vertex_init, const std::vector<Face>& faces_init)
    : vertices(vertex_init), faces(faces_init) {

}



void Mesh::readOFFFile( const string& filename, std::vector<Vertex>& vertices, std::vector<Face>& faces){
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
            switch (otherVertex ){
                case 1:
                    faces[otherFace].f1 = i ; 
                    vertices[faces[otherFace].v1].front_index = i ;
                    break;
                case 2: 
                    faces[otherFace].f2 = i ; 
                    vertices[faces[otherFace].v2].front_index = i ;
                    break;
                case 3 :
                    faces[otherFace].f3 = i ; 
                    vertices[faces[otherFace].v3].front_index = i ;
                    break;
            }
            faces[i].f3 = otherFace;
        } else{
            indexing_map[{face.v1 , face.v2 }] = { i , 3}; // cette aréte appartient au face i et la sommet qu'il faut mettre est v3
            indexing_map[{face.v2 , face.v1 }] = { i , 3};
        }

        iter = indexing_map.find({face.v1 , face.v3 }) ;
        if (iter != indexing_map.end()) {
             int otherFace = iter->second.first;  
            int otherVertex = iter->second.second; 
            switch (otherVertex ){
                case 1:
                    faces[otherFace].f1 = i ; 
                    vertices[faces[otherFace].v1].front_index = i ;
                    break;
                case 2: 
                    faces[otherFace].f2 = i ; 
                    vertices[faces[otherFace].v2].front_index = i ;
                    break;
                case 3 :
                    faces[otherFace].f3 = i ; 
                    vertices[faces[otherFace].v3].front_index = i ;
                    break;
            }
            faces[i].f2 = otherFace;
        } else{
            indexing_map[{face.v1 , face.v3 }] = { i , 2};
            indexing_map[{face.v3 , face.v1 }] = { i , 2};
        }

        iter = indexing_map.find({face.v2 , face.v3 }) ;
        if (iter != indexing_map.end()) {
            int otherFace = iter->second.first;  
            int otherVertex = iter->second.second; 
            switch (otherVertex ){
                case 1:
                    faces[otherFace].f1 = i ; 
                    vertices[faces[otherFace].v1].front_index = i ;
                    break;
                case 2: 
                    faces[otherFace].f2 = i ; 
                    vertices[faces[otherFace].v2].front_index = i ;
                    break;
                case 3 :
                    faces[otherFace].f3 = i ; 
                    vertices[faces[otherFace].v3].front_index = i ;
                    break;
            }
            faces[i].f1 = otherFace;
        } else{
            indexing_map[{face.v2 , face.v3 }] = { i , 1};
            indexing_map[{face.v3 , face.v2 }] = { i , 1};
        }
        std::cout << "Size of the map: " << indexing_map.size() << std::endl;
    }
 file.close();
}



void Mesh::saveOFFFile(const vector<Vertex>& vertices, const vector<Face>& faces, const string& filename) {
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

