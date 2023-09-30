
#include <iostream >
#include <vector>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include "Mesh.h"

int main()
{
    // CreatePyramid() ;
    std::vector<Face> faces(5);
    std::vector<Vertex> vertices(5);
    Mesh Pyramid(vertices, faces);
    Pyramid.readOFFFile("data/cube.off", Pyramid.vertices, Pyramid.faces);
    Pyramid.saveOFFFile(Pyramid.vertices, Pyramid.faces, "recon_cube.off");

    std::cout << "Vertices:" << std::endl;
    for (Mesh::Iterator_on_vertices it = Pyramid.vertices_begin(); it != Pyramid.vertices_past_the_end(); ++it)
    {
        const Vertex &vertex = *it;
        std::cout << "x: " << vertex.x << ", y: " << vertex.y << ", z: " << vertex.z << ", face equivalent: " << vertex.front_index << std::endl;
    }

    std::cout << "Faces:" << std::endl;
    for (Mesh::Iterator_on_faces it = Pyramid.faces_begin(); it != Pyramid.faces_past_the_end(); ++it)
    {
        const Face &face = *it;
        std::cout << "vertices: " << face.v1 << ", " << face.v2 << ",  " << face.v3 << ", faces : " << face.f1 << ",    " << face.f2 << ",   " << face.f3 << std::endl;
    }

    for (Mesh::Iterator_on_vertices its = Pyramid.vertices_begin(); its != Pyramid.vertices_past_the_end(); ++its)
    {
        Mesh::Circulator_on_faces cfbegin = Pyramid.incident_faces(*its);
        int cmpt = 1;
        Mesh::Circulator_on_faces cf2 = cfbegin;
        cf2++ ;
       
        for (Mesh::Circulator_on_faces cf = cf2; cf != cfbegin; ++cf)
        {
            cmpt++;
        }
        const Vertex &vertex = *its;
        std ::cout << "valence of the vertex " << cmpt << std ::endl;
    }

    return 0;
}

// working with maps :
//     indexing_map[{1, 2}] = {3, 4};
//     indexing_map[{5, 6}] = {7, 8};
//     indexing_map[{9, 10}] = {11, 12};

//     Access and print values from the map
//     std::pair<int, int> key = {1, 2};
//     std::map<std::pair<int, int>, std::pair<int, int>>::iterator it = indexing_map.find(key);

//     if (it != indexing_map.end()) {
//         std::pair<int, int> value = it->second;
//         std::cout << "Key: (" << key.first << ", " << key.second << "), Value: (" << value.first << ", " << value.second << ")\n";
//     } else {
//         std::cout << "Key not found in the map.\n";
//     }