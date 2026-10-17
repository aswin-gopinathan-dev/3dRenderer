#include "../inc/Mesh.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>


Mesh::Mesh()
{

}

Mesh::~Mesh()
{
}

void Mesh::CreateCubeMesh()
{
    mesh_vertices.clear();
    mesh_vertices.push_back({-1, -1, -1});
    mesh_vertices.push_back({-1,  1, -1});
    mesh_vertices.push_back({ 1,  1, -1});
    mesh_vertices.push_back({ 1, -1, -1});
    mesh_vertices.push_back({ 1,  1,  1});
    mesh_vertices.push_back({ 1, -1,  1});
    mesh_vertices.push_back({-1,  1,  1});
    mesh_vertices.push_back({-1, -1,  1});

    mesh_faces.clear();
    mesh_faces.push_back({1, 2, 3});
    mesh_faces.push_back({1, 3, 4});
    mesh_faces.push_back({4, 3, 5});
    mesh_faces.push_back({4, 5, 6});
    mesh_faces.push_back({6, 5, 7});
    mesh_faces.push_back({6, 7, 8});
    mesh_faces.push_back({8, 7, 2});
    mesh_faces.push_back({8, 2, 1});
    mesh_faces.push_back({2, 7, 5});
    mesh_faces.push_back({2, 5, 3});
    mesh_faces.push_back({6, 8, 1});
    mesh_faces.push_back({6, 1, 4});
}

void Mesh::CreateMeshFromObjFile(std::string filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
        throw std::runtime_error("Failed to open file: " + filename);

    mesh_vertices.clear();
    std::string line;
    while(std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            Vec3D vertex;
            if (iss >> vertex.x >> vertex.y >> vertex.z) {
                mesh_vertices.push_back(vertex);
            }
        }
        else if (prefix == "f") 
        {
            Face face {0, 0, 0};
            char slash;
            float temp;
            
            if (iss >> face.a >> slash >> temp >> slash >> temp
                    >> face.b >> slash >> temp >> slash >> temp
                    >> face.c >> slash >> temp >> slash >> temp)
            {
                mesh_faces.push_back(face);
            }
        }
    }
}
