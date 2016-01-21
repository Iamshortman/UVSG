#ifndef MATERIALMESH_HPP
#define MATERIALMESH_HPP

#include "openGL.hpp"
#include "glmInclude.hpp"
#include "Mesh.hpp"
#include <vector>

struct Material
{
	std::string name;
	vector3F diffuse_Color;
	//vector3F specular_Color;
	unsigned int illum_Value;
	//float shininess_Value;
	float alpha_Value;
};

struct MaterialVertex
{
	vector3F pos;
	vector3F normal;
	unsigned short material;
};

class MaterialMesh : public Mesh
{
public:
	MaterialMesh(std::vector<MaterialVertex>& vertices, std::vector<Material> materials);
	virtual ~MaterialMesh();

	virtual void draw(ShaderProgram* program);

private:
	GLuint vbo;
	int size;
	std::vector<Material> materials;
};

#endif //MATERIALMESH_HPP