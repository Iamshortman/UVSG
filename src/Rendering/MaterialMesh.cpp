#include "MaterialMesh.hpp"

MaterialMesh::MaterialMesh(std::vector<MaterialVertex>& vertices, std::vector<Material> materials)
{
	this->materials = materials;
	size = (int)vertices.size();

	//GenBuffers
	glGenBuffers(1, &vbo);

	//Adds the data to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MaterialVertex), &vertices[0], GL_STATIC_DRAW);
}


void MaterialMesh::draw(ShaderProgram* program)
{
	if (size == 0)
	{
		return;
	}

	for (int i = 0; i < materials.size(); i++)
	{
		std::string value = std::to_string(i);
		program->setUniform("materials[" + value + "].diffuse_Color", materials[i].diffuse_Color);
		program->setUniform("materials[" + value + "].alpha_Value", materials[i].alpha_Value);
		program->setUniform("materials[" + value + "].illum", materials[i].illum_Value);
	}

	//Enable Attributes
	glEnableVertexAttribArray(0); //Position
	glEnableVertexAttribArray(1); //Normal
	glEnableVertexAttribArray(2); //UV

	//Bind the buffer to begin drawing
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Position
	glVertexAttribPointer(
		0,                  //Position attribute location
		3,					//Position is a vec3
		GL_FLOAT,           //type
		GL_FALSE,           //normalized?
		sizeof(MaterialVertex),     // stride
		(void*)0            // array buffer offset
		);

	//Normal
	glVertexAttribPointer(
		1,                  //Normal attribute location
		3,					//Normal is a vec3
		GL_FLOAT,           //type
		GL_FALSE,          //normalized?
		sizeof(MaterialVertex),     // stride
		(void*)offsetof(MaterialVertex, normal) // array buffer offset
		);

	//Material
	glVertexAttribPointer(
		2,                  //UV attribute location
		1,					//Material is one float
		GL_FLOAT,	//type
		GL_FALSE,           //normalized?
		sizeof(MaterialVertex),     // stride
		(void*)offsetof(MaterialVertex, material) // array buffer offset
		);


	//Draw the mesh
	glDrawArrays(GL_TRIANGLES, 0, size);

	//Disable Attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

MaterialMesh::~MaterialMesh()
{
	glDeleteBuffers(1, &vbo);
}