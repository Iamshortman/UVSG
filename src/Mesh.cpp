#include "Mesh.hpp"


Mesh::Mesh()
{
    //We really don't do anything here
    this->size = 0;
}

void Mesh::addVertices(std::vector<vector3>& vertices, std::vector<vector3>& colors, std::vector<unsigned int>& indices)
{
    std::vector<vector3> normals = std::vector<vector3>();

    for(unsigned int i = 0; i < indices.size(); i += 3)
    {
        vector3 point1 = vertices[ indices[i] ];
        vector3 point2 = vertices[ indices[i + 1] ];
        vector3 point3 = vertices[ indices[i + 2] ];

        point2 -= point1;
        point3 -= point1;

        vector3 normal = glm::normalize( glm::cross(point2, point3) );
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
        //std::cout << normal.x << ", " <<  normal.y << "," << normal.z << std::endl;
    }
    addVertices(vertices, colors, normals, indices);
}

void Mesh::addVertices(std::vector<vector3>& vertices, std::vector<vector3>& colors, std::vector<vector3>& normals, std::vector<unsigned int>& indices)
{
	size = (int)indices.size();

    //Clear the old one incase
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &cbo);
    glDeleteBuffers(1, &nbo);
    glDeleteBuffers(1, &ibo);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &cbo);
    glGenBuffers(1, &nbo);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vector3), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vector3), &colors[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vector3), &normals[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void Mesh::addVertices(std::vector<vector3>& vertices, std::vector<vector3>& colors)
{
	size = (int)vertices.size();

	//Clear the old one incase
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &cbo);
	glDeleteBuffers(1, &nbo);
	glDeleteBuffers(1, &ibo);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &nbo);
	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vector3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vector3), &colors[0], GL_STATIC_DRAW);
}

void Mesh::draw()
{
        if(size == 0)
        {
            return;
        }

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );

        glBindBuffer(GL_ARRAY_BUFFER, cbo);
        glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
        );

        glBindBuffer(GL_ARRAY_BUFFER, nbo);
        glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

		// Draw the triangles !
		//glDrawArrays(GL_TRIANGLES, 0, size);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &cbo);
    glDeleteBuffers(1, &nbo);
    glDeleteBuffers(1, &ibo);
}
