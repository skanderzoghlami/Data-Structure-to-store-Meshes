#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

struct Vertex {
    float x, y , z ;
    int front_index  ;
} ;
struct Face{
    int v1,v2,v3 ;
    int f1,f2,f3 ; 
};

using std::vector ;
using std::string ;

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertex_init, const std::vector<Face>& faces_init);
    void saveOFFFile(const vector<Vertex>& vertices, const vector<Face>& faces, const string& filename);
    void readOFFFile( const string& filename, std::vector<Vertex>& vertices, std::vector<Face>& faces);
    

    std::vector<Vertex> vertices;
    std::vector<Face> faces;

    // Mr Iterator For Vertices
    class Iterator_on_vertices {
    public:
        // Constructor
        Iterator_on_vertices(std::vector<Vertex>::iterator it) : it_(it) {}

        // operators 
        Iterator_on_vertices& operator++() {
            ++it_;
            return *this;
        }

        Iterator_on_vertices operator++(int) {
            Iterator_on_vertices temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator_on_vertices& other) const {
            return it_ == other.it_;
        }

        bool operator!=(const Iterator_on_vertices& other) const {
            return it_ != other.it_;
        }

        Vertex& operator*() {
            return *it_;
        }

    private:
        std::vector<Vertex>::iterator it_;
    };
    Iterator_on_vertices vertices_begin() {
        return Iterator_on_vertices(vertices.begin());
    }

    Iterator_on_vertices vertices_past_the_end() {
        return Iterator_on_vertices(vertices.end());
    }
    // Mr Iterator For Faces
    class Iterator_on_faces {
    public:
        // Constructor
        Iterator_on_faces(std::vector<Face>::iterator it) : it_(it) {}

        // operators 
        Iterator_on_faces& operator++() {
            ++it_;
            return *this;
        }

        Iterator_on_faces operator++(int) {
            Iterator_on_faces temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator_on_faces& other) const {
            return it_ == other.it_;
        }

        bool operator!=(const Iterator_on_faces& other) const {
            return it_ != other.it_;
        }

        Face& operator*() {
            return *it_;
        }

    private:
        std::vector<Face>::iterator it_;
    };
    Iterator_on_faces faces_begin() {
        return Iterator_on_faces(faces.begin());
    }

    Iterator_on_faces faces_past_the_end() {
        return Iterator_on_faces(faces.end());
    }
        // Mr Circulator on faces
    class Circulator_on_faces {
    public:
        // Constructor
        Circulator_on_faces(std::vector<Face>::iterator it) : it_(it) {}

        // operators 
        Circulator_on_faces& operator++() {
            ++it_;
            return *this;
        }

        Circulator_on_faces operator++(int) {
            Circulator_on_faces temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Circulator_on_faces& other) const {
            return it_ == other.it_;
        }

        bool operator!=(const Circulator_on_faces& other) const {
            return it_ != other.it_;
        }

        Face& operator*() {
            return *it_;
        }

    private:
        std::vector<Face>::iterator it_;
    };
    Circulator_on_faces incident_faces(Vertex& v) ;
};

#endif // MESH_H
