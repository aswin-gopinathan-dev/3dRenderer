#include "../inc/Mesh.h"

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
