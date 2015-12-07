#include "ColoredMesh.hpp"

ColoredMesh::ColoredMesh(std::vector<ColoredVertex>& vertices, std::vector<unsigned int>& indices)
{
	size = (int)indices.size();

	//GenBuffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	//Adds the data to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ColoredVertex), &vertices[0], GL_STATIC_DRAW);

	//Adds the indices to the buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}


void ColoredMesh::draw()
{
	if (size == 0)
	{
		return;
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
		sizeof(ColoredVertex),     // stride
		(void*)0            // array buffer offset
		);

	//Normal
	glVertexAttribPointer(
		1,                  //Normal attribute location
		3,					//Normal is a vec3
		GL_FLOAT,           //type
		GL_FALSE ,          //normalized?
		sizeof(ColoredVertex),     // stride
		(void*)offsetof(ColoredVertex, normal) // array buffer offset
		);

	//UV
	glVertexAttribPointer(
		2,                  //UV attribute location
		3,					//color is a vec3
		GL_FLOAT,           //type
		GL_FALSE,           //normalized?
		sizeof(ColoredVertex),     // stride
		(void*)offsetof(ColoredVertex, color) // array buffer offset
		);


	//Draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

	//Disable Attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

ColoredMesh::~ColoredMesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}