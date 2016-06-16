#define _CRT_SECURE_NO_DEPRECATE

#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include <btBulletCollisionCommon.h>
#include <string>
#include <cstring>
#include <vector>

#include "GLM_Include.hpp"
#include "Rendering/Model/TexturedMesh.hpp"
#include "Rendering/Model/MaterialMesh.hpp"

bool loadOBJ(const char * path, std::vector<vector3F> & out_vertices, std::vector<vector2F> & out_uvs, std::vector<vector3F> & out_normals);
TexturedMesh* loadMeshFromFile(std::string filePath);

MaterialMesh* loadMaterialMeshFromFile(std::string filePath, std::string fileName);
bool loadMaterial(std::string filePath, std::vector<Material> &materials);
bool loadMaterialOBJ(std::string fileName, std::vector<vector3F> & out_vertices, std::vector<vector3F> & out_normals, std::vector<unsigned short> & out_material, std::vector<Material> & materials, std::string filePath);
void loadTriMesh(std::string filePath, std::string fileName, btTriangleMesh* triMesh);
void loadConvexHull(std::string filePath, std::string fileName, btConvexHullShape* shape);


#endif //OBJLOADER_HPP
