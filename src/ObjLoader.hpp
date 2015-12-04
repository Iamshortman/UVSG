#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include <string>
#include <vector>
#include "glmInclude.hpp"
#include "TexturedMesh.hpp"

bool loadOBJ(const char * path, std::vector<vector3> & out_vertices, std::vector<vector2> & out_uvs, std::vector<vector3> & out_normals);
TexturedMesh* loadMeshFromFile(std::string filepath);

#endif //OBJLOADER_HPP