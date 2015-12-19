#include "NonIndexedColoredMesh.hpp"

NonIndexedColoredMesh::NonIndexedColoredMesh(std::vector<NonIndexedColoredVertex>& vertices)
{
	size = (int)vertices.size();

	//GenBuffers
	glGenBuffers(1, &vbo);

	//Adds the data to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(NonIndexedColoredVertex), &vertices[0], GL_STATIC_DRAW);
}


void NonIndexedColoredMesh::draw()
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
		sizeof(NonIndexedColoredVertex),     // stride
		(void*)0            // array buffer offset
		);

	//Normal
	glVertexAttribPointer(
		1,                  //Normal attribute location
		3,					//Normal is a vec3
		GL_FLOAT,           //type
		GL_FALSE ,          //normalized?
		sizeof(NonIndexedColoredVertex),     // stride
		(void*)offsetof(NonIndexedColoredVertex, normal) // array buffer offset
		);

	//UV
	glVertexAttribPointer(
		2,                  //UV attribute location
		3,					//color is a vec3
		GL_FLOAT,           //type
		GL_FALSE,           //normalized?
		sizeof(NonIndexedColoredVertex),     // stride
		(void*)offsetof(NonIndexedColoredVertex, color) // array buffer offset
		);


	//Draw the mesh
	glDrawArrays(GL_TRIANGLES, 0, size);

	//Disable Attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

NonIndexedColoredMesh::~NonIndexedColoredMesh()
{
	glDeleteBuffers(1, &vbo);
}