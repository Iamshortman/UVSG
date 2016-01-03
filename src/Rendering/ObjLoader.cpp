#include "ObjLoader.hpp"Thruster.obj

bool loadOBJ(const char * path, std::vector<vector3F> & out_vertices, std::vector<vector2F> & out_uvs, std::vector<vector3F> & out_normals)
{
	FILE * file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Cannot Open File: %s \n", path);
		return false;
	}

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< vector3F > temp_vertices;
	std::vector< vector2F > temp_uvs;
	std::vector< vector3F > temp_normals;

	while (1)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0)
		{
			vector3F vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			vector2F uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vector3F normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				printf("Could not Create Faces!!!! \n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		vector3F vertex = temp_vertices[vertexIndex - 1];
		vector2F uv = temp_uvs[uvIndex - 1];
		vector3F normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
	}

	return true;
}

TexturedMesh* loadMeshFromFile(std::string filepath)
{
	std::vector<vector3F> vertices;
	std::vector<vector2F> uvs;
	std::vector<vector3F> normals;
	if (loadOBJ(filepath.c_str(), vertices, uvs, normals))
	{
		std::vector<TexturedVertex> vertexVector;
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < vertices.size(); i++)
		{
			vertexVector.push_back({vertices[i], normals[i], uvs[i]});
			indices.push_back(i);
		}

		return new TexturedMesh(vertexVector, indices);
	}

	return 0;
}
