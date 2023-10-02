#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

struct Vertex
{
    float x, y, z;
    int front_index;
    bool isFinite = true ;
    bool operator==(const Vertex &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};
struct Face
{
    int v1, v2, v3;
    int f1, f2, f3;
    bool isFinite = true ;
    bool operator==(const Face &other) const
    {
        return v1 == other.v1 && v2 == other.v2 && v3 == other.v3;
    }
    bool operator!=(const Face &other) const
    {
        return v1 != other.v1 || v2 != other.v2 || v3 != other.v3;
    }
};

using std::string;
using std::vector;

class Mesh
{
public:
    Mesh(const std::vector<Vertex> &vertex_init, const std::vector<Face> &faces_init);
    void saveOFFFile(const vector<Vertex> &vertices, const vector<Face> &faces, const string &filename);
    void readOFFFile(const string &filename, std::vector<Vertex> &vertices, std::vector<Face> &faces);
    void addSommet(int indiceFace , Vertex& new_vetex );
    void edgeFlip(int f1 , int f2);
    float test_orientation(Vertex v1 , Vertex v2 , Vertex v3) ;
    int pointInTriangle( Vertex& A,  Vertex& B,  Vertex& C,  Vertex& P);
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    
    // Mr Iterator For Vertices
    class Iterator_on_vertices
    {
    public:
        // Constructor
        Iterator_on_vertices(std::vector<Vertex>::iterator it) : it_(it) {}

        // operators
        Iterator_on_vertices &operator++()
        {
            ++it_;
            return *this;
        }

        Iterator_on_vertices operator++(int)
        {
            Iterator_on_vertices temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator_on_vertices &other) const
        {
            return it_ == other.it_;
        }

        bool operator!=(const Iterator_on_vertices &other) const
        {
            return it_ != other.it_;
        }

        Vertex &operator*()
        {
            return *it_;
        }

    private:
        std::vector<Vertex>::iterator it_;
    };
    Iterator_on_vertices vertices_begin()
    {
        return Iterator_on_vertices(vertices.begin());
    }

    Iterator_on_vertices vertices_past_the_end()
    {
        return Iterator_on_vertices(vertices.end());
    }
    // Mr Iterator For Faces
    class Iterator_on_faces
    {
    public:
        // Constructor
        Iterator_on_faces(std::vector<Face>::iterator it) : it_(it) {}

        // operators
        Iterator_on_faces &operator++()
        {
            ++it_;
            return *this;
        }

        Iterator_on_faces operator++(int)
        {
            Iterator_on_faces temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator_on_faces &other) const
        {
            return it_ == other.it_;
        }

        bool operator!=(const Iterator_on_faces &other) const
        {
            return it_ != other.it_;
        }

        Face &operator*()
        {
            return *it_;
        }

    private:
        std::vector<Face>::iterator it_;
    };

    Iterator_on_faces faces_begin()
    {
        return Iterator_on_faces(faces.begin());
    }

    Iterator_on_faces faces_past_the_end()
    {
        return Iterator_on_faces(faces.end());
    }

    // Mr Circulator on faces
    class Circulator_on_faces
    {
    public:
        // Constructor
        Circulator_on_faces(Face &it, Vertex &vertex, Mesh &mesh) : it_(it), sommet(vertex), mesh(mesh) {}

        // operators
        Circulator_on_faces operator++()
        {
            if (mesh.vertices[it_.v1] == sommet)
            {
                it_ = mesh.faces[it_.f2];
                
                return *this;
            }
            else if (mesh.vertices[it_.v2] == sommet)
            {
                it_ = mesh.faces[it_.f3];
                return *this;
            }
            it_ = mesh.faces[it_.f1];
            return *this;
        }

        Circulator_on_faces operator++(int)
        {
            Circulator_on_faces temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Circulator_on_faces &other) const
        {
            return it_ == other.it_;
        }

        bool operator!=(const Circulator_on_faces &other) const
        {
            return it_ != other.it_;
        }
        Face &operator*()
        {
            return it_;
        }

    public:
        Face it_;
        Vertex sommet;
        Mesh &mesh;
    };

    Mesh::Circulator_on_faces incident_faces(Vertex &v)
    {
        for (Mesh::Iterator_on_faces it = faces_begin(); it != faces_past_the_end(); ++it)
        {
            Face face = *it;
            if ((vertices[face.v1] == v) || (vertices[face.v2] == v) || (vertices[face.v3] == v))
            {
                return Mesh::Circulator_on_faces(face, v, *this);
            }
        }
    }
};

#endif // MESH_H
