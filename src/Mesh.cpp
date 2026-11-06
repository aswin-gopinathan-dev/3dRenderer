#include "../inc/Mesh.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>


Mesh::Mesh() : translation(0, 0, 0), scale(1.0, 1.0, 1.0)
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
    mesh_faces.push_back({1, 2, 3, 0xFFFFFFFF});
    mesh_faces.push_back({1, 3, 4, 0xFFFFFFFF});
    mesh_faces.push_back({4, 3, 5, 0xFFFFFFFF});
    mesh_faces.push_back({4, 5, 6, 0xFFFFFFFF});
    mesh_faces.push_back({6, 5, 7, 0xFFFFFFFF});
    mesh_faces.push_back({6, 7, 8, 0xFFFFFFFF});
    mesh_faces.push_back({8, 7, 2, 0xFFFFFFFF});
    mesh_faces.push_back({8, 2, 1, 0xFFFFFFFF});
    mesh_faces.push_back({2, 7, 5, 0xFFFFFFFF});
    mesh_faces.push_back({2, 5, 3, 0xFFFFFFFF});
    mesh_faces.push_back({6, 8, 1, 0xFFFFFFFF});
    mesh_faces.push_back({6, 1, 4, 0xFFFFFFFF});
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
            Face face {0, 0, 0, 0xFFFFFFFF};
            char slash;
            float temp;
            
            if (iss >> face.a >> slash >> temp >> slash >> temp
                    >> face.b >> slash >> temp >> slash >> temp
                    >> face.c >> slash >> temp >> slash >> temp)
            {
                face.color = 0xFFFFFFFF;
                mesh_faces.push_back(face);
            }
        }
    }
}
