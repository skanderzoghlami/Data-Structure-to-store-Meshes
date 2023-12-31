#include <iostream >
#include <vector>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include "Mesh.h"

using std::cout;
using std::endl;
using std::vector;
using namespace std;

Mesh::Mesh(const std::vector<Vertex> &vertex_init, const std::vector<Face> &faces_init)
    : vertices(vertex_init), faces(faces_init)
{
}

void Mesh::readOFFFile(const string &filename, std::vector<Vertex> &vertices, std::vector<Face> &faces)
{
    std::ifstream file(filename);
    std::string line;

    int numVertices, numFaces, numEdges;
    string holder;
    file >> holder;
    file >> numVertices >> numFaces >> numEdges;
    vertices.resize(numVertices);
    faces.resize(numFaces);

    map<pair<int, int>, pair<int, int>> indexing_map;

    for (int i = 0; i < numVertices; ++i)
    {
        Vertex vertex;
        file >> vertex.x >> vertex.y >> vertex.z;
        vertex.front_index = -1;
        vertices[i] = vertex;
    }

    for (int i = 0; i < numFaces; ++i)
    {
        Face face;

        face.f1 = -1;
        face.f2 = -1;
        face.f3 = -1;

        int numVerticesInFace;
        file >> numVerticesInFace >> face.v1 >> face.v2 >> face.v3;

        vertices[face.v1].front_index = i;
        vertices[face.v2].front_index = i;
        vertices[face.v3].front_index = i;

        faces[i] = face;

        std::map<std::pair<int, int>, std::pair<int, int>>::iterator iter = indexing_map.find({face.v1, face.v2});

        if (iter != indexing_map.end())
        {
            int otherFace = iter->second.first;
            int otherVertex = iter->second.second;
            switch (otherVertex)
            {
            case 1:
                faces[otherFace].f1 = i;
                break;
            case 2:
                faces[otherFace].f2 = i;
                break;
            case 3:
                faces[otherFace].f3 = i;
                break;
            }
            faces[i].f3 = otherFace;
        }
        else
        {
            indexing_map[{face.v1, face.v2}] = {i, 3}; // cette aréte appartient au face i et la sommet qu'il faut mettre est v3
            indexing_map[{face.v2, face.v1}] = {i, 3};
        }

        iter = indexing_map.find({face.v1, face.v3});
        if (iter != indexing_map.end())
        {
            int otherFace = iter->second.first;
            int otherVertex = iter->second.second;
            switch (otherVertex)
            {
            case 1:
                faces[otherFace].f1 = i;
                break;
            case 2:
                faces[otherFace].f2 = i;
                break;
            case 3:
                faces[otherFace].f3 = i;
                break;
            }
            faces[i].f2 = otherFace;
        }
        else
        {
            indexing_map[{face.v1, face.v3}] = {i, 2};
            indexing_map[{face.v3, face.v1}] = {i, 2};
        }

        iter = indexing_map.find({face.v2, face.v3});
        if (iter != indexing_map.end())
        {
            int otherFace = iter->second.first;
            int otherVertex = iter->second.second;
            switch (otherVertex)
            {
            case 1:
                faces[otherFace].f1 = i;
                break;
            case 2:
                faces[otherFace].f2 = i;
                break;
            case 3:
                faces[otherFace].f3 = i;
                break;
            }
            faces[i].f1 = otherFace;
        }
        else
        {
            indexing_map[{face.v2, face.v3}] = {i, 1};
            indexing_map[{face.v3, face.v2}] = {i, 1};
        }
    }
    file.close();
}

void Mesh::saveOFFFile(const vector<Vertex> &vertices, const vector<Face> &faces, const string &filename)
{
    ofstream outFile(filename);
    if (!outFile.is_open())
    {
        cerr << "Error: Unable to open file " << filename << " for writing." << endl;
        return;
    }

    outFile << "OFF" << endl;
    outFile << vertices.size() << " " << faces.size() << " 0" << endl;
    for (const Vertex &vertex : vertices)
    {
        outFile << vertex.x << " " << vertex.y << " " << vertex.z << endl;
    }
    for (const Face &face : faces)
    {
        outFile << "3 " << face.v1 << " " << face.v2 << " " << face.v3 << endl;
    }
    outFile.close();
}

void Mesh::addSommet(int indiceFace, Vertex &new_vetex)
{

    // Vertices Configuration
    new_vetex.front_index = indiceFace;
    this->vertices.push_back(new_vetex);
    int new_v_place = vertices.size() - 1;
    Face &nf0 = this->faces[indiceFace];
    Face nf1;
    nf1.v1 = nf0.v2, nf1.v2 = nf0.v3, nf1.v3 = new_v_place;
    Face nf2;
    nf2.v1 = nf0.v3, nf2.v2 = nf0.v1, nf2.v3 = new_v_place;
    nf0.v3 = new_v_place;
    // Faces configuraton

    int pos_nf1 = faces.size();
    int pos_nf2 = faces.size() + 1;
    int oldface0 = nf0.f1, oldface1 = nf0.f2, oldface2 = nf0.f3;

    nf0.f1 = pos_nf1, nf0.f2 = pos_nf2, nf0.f3 = oldface2;
    nf1.f1 = pos_nf2, nf1.f2 = indiceFace, nf1.f3 = oldface0;
    nf2.f1 = indiceFace, nf2.f2 = pos_nf1, nf2.f3 = oldface1;

    this->faces.push_back(nf1);
    this->faces.push_back(nf2);
}
void Mesh::edgeFlip(int f1, int f2)
{

    Face &t1 = this->faces[f1];
    Face &t2 = this->faces[f2];

    // Find the shared vertices between the two faces.
    int sharedV1 = -1, sharedV2 = -1, faceV11 = -1, faceV12 = -1, faceV21 = -1, faceV22 = -1;

    if (t1.v1 == t2.v1 || t1.v1 == t2.v2 || t1.v1 == t2.v3)
    {
        sharedV1 = t1.v1;
        faceV11 = t1.f1;
        if (t1.v1 == t2.v1)
            faceV12 = t2.f1;
        if (t1.v1 == t2.v2)
            faceV12 = t2.f2;
        if (t1.v1 == t2.v3)
            faceV12 = t2.f3;
    }
    if (t1.v2 == t2.v1 || t1.v2 == t2.v2 || t1.v2 == t2.v3)
    {
        sharedV1 = t1.v2;
        faceV11 = t1.f2;
        if (t1.v2 == t2.v1)
            faceV12 = t2.f1;
        if (t1.v2 == t2.v2)
            faceV12 = t2.f2;
        if (t1.v2 == t2.v3)
            faceV12 = t2.f3;
    }
    if (t1.v3 == t2.v1 || t1.v3 == t2.v2 || t1.v3 == t2.v3)
    {
        sharedV1 = t1.v3;
        faceV11 = t1.f3;
        if (t1.v3 == t2.v1)
            faceV12 = t2.f1;
        if (t1.v3 == t2.v2)
            faceV12 = t2.f2;
        if (t1.v3 == t2.v3)
            faceV12 = t2.f3;
    }

    // Find the other shared vertex.
    if (t2.v1 != sharedV1 && (t2.v1 == t1.v1 || t2.v1 == t1.v2 || t2.v1 == t1.v3))
    {
        sharedV2 = t2.v1;
        faceV21 = t2.f1;
        if (t2.v1 == t1.v1)
            faceV22 = t1.f1;
        if (t2.v1 == t1.v2)
            faceV22 = t1.f2;
        if (t2.v1 == t1.v3)
            faceV22 = t1.f3;
    }
    if (t2.v2 != sharedV1 && (t2.v2 == t1.v1 || t2.v2 == t1.v2 || t2.v2 == t1.v3))
    {
        sharedV2 = t2.v2;
        faceV21 = t2.f2;
        if (t2.v2 == t1.v1)
            faceV22 = t1.f1;
        if (t2.v2 == t1.v2)
            faceV22 = t1.f2;
        if (t2.v2 == t1.v3)
            faceV22 = t1.f3;
    }
    if (t2.v3 != sharedV1 && (t2.v3 == t1.v1 || t2.v3 == t1.v2 || t2.v3 == t1.v3))
    {
        sharedV2 = t2.v3;
        faceV21 = t2.f3;
        if (t2.v3 == t1.v1)
            faceV22 = t1.f1;
        if (t2.v3 == t1.v2)
            faceV22 = t1.f2;
        if (t2.v3 == t1.v3)
            faceV22 = t1.f3;
    }

    int nonShared1, nonShared2;
    if (t1.v1 != sharedV1)
        nonShared1 = t1.v1;
    if (t1.v2 != sharedV1)
        nonShared1 = t1.v2;
    if (t1.v3 != sharedV1)
        nonShared1 = t1.v3;

    if (t2.v1 != sharedV1)
        nonShared2 = t2.v1;
    if (t2.v2 != sharedV1)
        nonShared2 = t2.v2;
    if (t2.v3 != sharedV1)
        nonShared2 = t2.v3;
    if (Mesh::test_orientation(vertices[nonShared1], vertices[sharedV1], vertices[nonShared2]))
    {
        t1.v1 = nonShared1;
        t1.v2 = sharedV1;
        t1.v3 = nonShared2;

        t2.v1 = nonShared2;
        t2.v2 = sharedV2;
        t2.v3 = nonShared1;
    }
    else
    {
        t1.v1 = nonShared2;
        t1.v2 = sharedV1;
        t1.v3 = nonShared1;

        t2.v1 = nonShared1;
        t2.v2 = sharedV2;
        t2.v3 = nonShared2;
    }

    int n[4] = {faceV11 , faceV12 , faceV21 ,faceV22} ;
    for (int i = 0 ; i<4 ; i++ ){
        bool b1,b2,b3 ;
       if( areNeighbors(t1 , faces[n[i]], b1,b2,b3)){
        if(!b1) t1.f1 = n[i];
        if(!b2) t1.f2 = n[i];
        if(!b3) t1.f3 = n[i];
       }
       if( areNeighbors(t2 , faces[n[i]], b1,b2,b3)){
        if(!b1) t2.f1 = n[i];
        if(!b2) t2.f2 = n[i];
        if(!b3) t2.f3 = n[i];
       }
    }
}

float Mesh::test_orientation(Vertex v1, Vertex v2, Vertex v3)
{
    float v1v2_x = v2.x - v1.x;
    float v1v2_y = v2.y - v1.y;
    float v1v3_x = v3.x - v1.x;
    float v1v3_y = v3.y - v1.y;
    float result = v1v2_x * v1v3_y - v1v2_y * v1v3_x;
    if (result > 0)
    {
        return 1; // Counterclockwise
    }
    else if (result < 0)
    {
        return -1; // Clockwise
    }
    else
    {
        return 0; // Collinéaire
    }
}

int Mesh::pointInTriangle(Vertex &A, Vertex &B, Vertex &C, Vertex &P)
{
    double orientation1 = this->test_orientation(A, B, P);
    double orientation2 = this->test_orientation(A, B, P);
    double orientation3 = this->test_orientation(A, B, P);

    if (orientation1 == 0.0 || orientation2 == 0.0 || orientation3 == 0.0)
    {
        return 0; // On the boundary
    }
    else if ((orientation1 > 0 && orientation2 > 0 && orientation3 > 0) ||
             (orientation1 < 0 && orientation2 < 0 && orientation3 < 0))
    {
        return 1; // Inside the triangle
    }
    else
    {
        return -1; // Outside the triangle
    }
}

bool areNeighbors(const Face &face1, const Face &face2, bool &v1Shared, bool &v2Shared, bool &v3Shared)
{
    v1Shared = (face1.v1 == face2.v1 || face1.v1 == face2.v2 || face1.v1 == face2.v3);
    v2Shared = (face1.v2 == face2.v1 || face1.v2 == face2.v2 || face1.v2 == face2.v3);
    v3Shared = (face1.v3 == face2.v1 || face1.v3 == face2.v2 || face1.v3 == face2.v3);

    // Calculate the number of shared vertices.
    int sharedVertexCount = v1Shared + v2Shared + v3Shared;

    // If there are two shared vertices, these faces share an edge.
    if (sharedVertexCount == 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}