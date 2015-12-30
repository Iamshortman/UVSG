#define _CRT_SECURE_NO_DEPRECATE

#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include <string>
#include <cstring>
#include <vector>
#include "glmInclude.hpp"
#include "TexturedMesh.hpp"

bool loadOBJ(const char * path, std::vector<vector3F> & out_vertices, std::vector<vector2F> & out_uvs, std::vector<vector3F> & out_normals);
TexturedMesh* loadMeshFromFile(std::string filepath);

#endif //OBJLOADER_HPP
