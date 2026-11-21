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
    mesh_faces.push_back({1, 2, 3, Texture{0, 1}, Texture{0, 0}, Texture{1, 0}, 0xFFFFFFFF});
    mesh_faces.push_back({1, 3, 4, Texture{0, 1}, Texture{1, 0}, Texture{1, 1}, 0xFFFFFFFF});
    mesh_faces.push_back({4, 3, 5, Texture{0, 1}, Texture{0, 0}, Texture{1, 0}, 0xFFFFFFFF});
    mesh_faces.push_back({4, 5, 6, Texture{0, 1}, Texture{1, 0}, Texture{1, 1}, 0xFFFFFFFF});
    mesh_faces.push_back({6, 5, 7, Texture{0, 1}, Texture{0, 0}, Texture{1, 0}, 0xFFFFFFFF});
    mesh_faces.push_back({6, 7, 8, Texture{0, 1}, Texture{1, 0}, Texture{1, 1}, 0xFFFFFFFF});
    mesh_faces.push_back({8, 7, 2, Texture{0, 1}, Texture{0, 0}, Texture{1, 0}, 0xFFFFFFFF});
    mesh_faces.push_back({8, 2, 1, Texture{0, 1}, Texture{1, 0}, Texture{1, 1}, 0xFFFFFFFF});
    mesh_faces.push_back({2, 7, 5, Texture{0, 1}, Texture{0, 0}, Texture{1, 0}, 0xFFFFFFFF});
    mesh_faces.push_back({2, 5, 3, Texture{0, 1}, Texture{1, 0}, Texture{1, 1}, 0xFFFFFFFF});
    mesh_faces.push_back({6, 8, 1, Texture{0, 1}, Texture{0, 0}, Texture{1, 0}, 0xFFFFFFFF});
    mesh_faces.push_back({6, 1, 4, Texture{0, 1}, Texture{1, 0}, Texture{1, 1}, 0xFFFFFFFF});
}

void Mesh::CreateMeshFromObjFile(std::string filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
        throw std::runtime_error("Failed to open file: " + filename);

    mesh_vertices.clear();
    mesh_faces.clear();

    std::vector<Texture> texcoords;

    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;

        iss >> prefix;

        if (prefix == "v")
        {
            Vec3D vertex;

            if (iss >> vertex.x >> vertex.y >> vertex.z)
                mesh_vertices.push_back(vertex);
        }
        else if (prefix == "vt")
        {
            float u, v;

            if (iss >> u >> v)
                texcoords.emplace_back(u, v);
        }
        else if (prefix == "f")
        {
            int vi[3], ti[3], ni[3];
            char slash;

            bool valid = true;

            for (int i = 0; i < 3; i++)
            {
                if (!(iss >> vi[i] >> slash >> ti[i]
                          >> slash >> ni[i]))
                {
                    valid = false;
                    break;
                }
            }

            if (!valid)
                continue;

            Face face {
                vi[0],
                vi[1],
                vi[2],
                texcoords.at(ti[0] - 1),
                texcoords.at(ti[1] - 1),
                texcoords.at(ti[2] - 1),
                0xFFFFFFFF
            };

            mesh_faces.push_back(face);
        }
    }
}
