#ifndef TESTCHUNK_HPP
#define TESTCHUNK_HPP

#include "Mesh.hpp"
#include "TexturedMesh.hpp"
#include "RigidBody.hpp"
#include "Components.hpp"

class TestChunk
{
public:
	const static int chunkSize = 16;

	int chunk[chunkSize][chunkSize];

	Mesh* cube = nullptr;

	vector2 offset = vector2(chunkSize / 2);

	TestChunk()
	{
		for (unsigned int x = 0; x < chunkSize; x++)
		{
			for (unsigned int y = 0; y < chunkSize; y++)
			{
				chunk[x][y] = 1;
			}
		}
	
	}

	void update(TimeDelta delta)
	{
		const Uint8 *state = SDL_GetKeyboardState(NULL);

		if (state[SDL_SCANCODE_W])
		{
			offset += vector2(1.0f * delta, 0.0f);
		}
		if (state[SDL_SCANCODE_S])
		{
			offset += -vector2(1.0f * delta, 0.0f);
		}

		if (state[SDL_SCANCODE_SPACE])
		{
			int x = offset.x;
			int y = offset.y;

			if (x >= 0 && x < chunkSize)
			{
				if (y >= 0 && y < chunkSize)
				{
					if (chunk[x][y] == 1)
					{
						chunk[x][y] = 0;
					}
					else if (chunk[x][y] == 0)
					{
						chunk[x][y] = 1;
					}
				}
			}


		}

	}

	void Render(ShaderProgram* shader, Camera* camera)
	{
		Transform transform;
		matrix4 modelMatrix = transform.getModleMatrix();
		matrix3 normalMatrix = transform.getNormalMatrix();

		matrix4 MVP = camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix;

		shader->setActiveProgram();

		shader->setUniform("MVP", MVP);
		shader->setUniform("normalMatrix", normalMatrix);

		for (unsigned int x = 0; x < chunkSize; x++)
		{
			for (unsigned int y = 0; y < chunkSize; y++)
			{
				if (chunk[x][y] != 0)
				{
					shader->setUniform("offset", vector3(x * 2, 0, y * 2));

					cube->draw();
				}
			}
		}

		shader->deactivateProgram();
	}

};


#endif TESTCHUNK_HPP