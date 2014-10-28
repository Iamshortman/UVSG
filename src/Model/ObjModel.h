//C style string class
#include <string.h>

#include <string>
#include <stdio.h>
#include <vector>
#include "../glmInclude.hpp"

bool loadOBJ(const char * path,
	std::vector<vector3> & out_vertices,
	std::vector<vector2> & out_uvs,
	std::vector<vector3> & out_normals);

