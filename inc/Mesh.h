#pragma once
#include <vector>
#include "Vector.h"
#include "Triangle.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
    std::vector<Vec3D>& GetMeshVertices() { return mesh_vertices; }
    std::vector<Face>& GetMeshFaces() { return mesh_faces; }

private:
    std::vector<Vec3D> mesh_vertices;
    std::vector<Face> mesh_faces;
};
	
