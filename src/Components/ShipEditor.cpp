#include "ShipEditor.hpp"
#include <SDL2/SDL.h>
#include "Util.hpp"

ShipEditor::ShipEditor()
{
	m_cursorPos = vector3S(0);
}

void ShipEditor::Update()
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	static Uint8 lastState_UP = 0;
	if (state[SDL_SCANCODE_UP] != 0 && lastState_UP == 0)
	{
		m_cursorPos.z += 1;
	}
	lastState_UP = state[SDL_SCANCODE_UP];

	static Uint8 lastState_DOWN = 0;
	if (state[SDL_SCANCODE_DOWN] != 0 && lastState_DOWN == 0)
	{
		m_cursorPos.z += -1;
	}
	lastState_DOWN = state[SDL_SCANCODE_DOWN];

	static Uint8 lastState_LEFT = 0;
	if (state[SDL_SCANCODE_LEFT] != 0 && lastState_LEFT == 0)
	{
		m_cursorPos.x += 1;
	}
	lastState_LEFT = state[SDL_SCANCODE_LEFT];

	static Uint8 lastState_RIGHT = 0;
	if (state[SDL_SCANCODE_RIGHT] != 0 && lastState_RIGHT == 0)
	{
		m_cursorPos.x += -1;
	}
	lastState_RIGHT = state[SDL_SCANCODE_RIGHT];

	static Uint8 lastState_SPACE = 0;
	if (state[SDL_SCANCODE_SPACE] != 0 && lastState_SPACE == 0)
	{
		if (!hasCell(m_cursorPos))
		{
			addCell(m_cursorPos);
		}
		else
		{
			removeCell(m_cursorPos);
		}
	}
	lastState_SPACE = state[SDL_SCANCODE_SPACE];

	if (needsUpdate)
	{
		this->updateOutsideMesh();
		this->updateInsideMesh();
		needsUpdate = false;
	}
}

void ShipEditor::TempRender(Camera& camera, TexturePool& texturePool)
{
	Transform transform;

	vector3D camPos = camera.getPos();
	vector3D offsetPos = transform.getPos() - camPos;
	vector3D scale = transform.getScale();

	vector3F floatPos = (vector3F)offsetPos;
	vector3F floatScale = (vector3F)scale;

	matrix4 m_positionMatrix = matrix4();
	matrix4 m_scaleMatrix = matrix4();
	m_positionMatrix = glm::translate(matrix4(1.0F), floatPos);
	m_scaleMatrix = glm::scale(matrix4(1.0F), floatScale);
	matrix4 modModelMatrix = m_positionMatrix * m_scaleMatrix;
	
	matrix4 MVP = camera.getProjectionMatrix() * camera.getOriginViewMatrix() * modModelMatrix;
	matrix3 normalMatrix = transform.getNormalMatrix();

	shader->setActiveProgram();
	shader->setUniform("MVP", MVP);
	shader->setUniform("normalMatrix", normalMatrix);
	shader->setUniform("localOffset", matrix4(1.0f));
	if (m_OutsideMesh != nullptr)
	{
		//this->m_OutsideMesh->draw();
	}

	if (m_InsideMesh != nullptr)
	{
		this->m_InsideMesh->draw();
	}

	shader->deactivateProgram();

	tempModel->shader->setActiveProgram();
	tempModel->shader->setUniform("MVP", MVP);
	tempModel->shader->setUniform("normalMatrix", normalMatrix);
	texturePool.bindTexture("res/arrow-up.png");
	Transform localTransform;
	localTransform.setPos(m_cursorPos);
	localTransform.setPos(localTransform.getPos() * (double)cubeSize);
	localTransform.setScale(vector3D(cubeSize));
	tempModel->shader->setUniform("localOffset", localTransform.getModleMatrix());
	tempModel->mesh->draw();

	tempModel->shader->deactivateProgram();
}

void ShipEditor::addCell(vector3S pos)
{
	m_shipCells[pos] = 1;
	needsUpdate = true;
}
void ShipEditor::removeCell(vector3S pos)
{
	m_shipCells.erase(pos);
	needsUpdate = true;
}

bool ShipEditor::hasCell(vector3S pos)
{
	return m_shipCells.find(pos) != m_shipCells.end();
}

void ShipEditor::updateOutsideMesh()
{
	if (m_OutsideMesh != 0)
	{
		delete m_OutsideMesh;
		m_OutsideMesh = nullptr;
	}

	vector3F vertsCube[] =
	{
		vector3F(-0.5f, -0.5f, -0.5f) * cubeSize,
		vector3F(-0.5f, -0.5f,  0.5f) * cubeSize,
		vector3F(-0.5f,  0.5f, -0.5f) * cubeSize,
		vector3F(-0.5f,  0.5f,  0.5f) * cubeSize,
		vector3F( 0.5f, -0.5f, -0.5f) * cubeSize,
		vector3F( 0.5f, -0.5f,  0.5f) * cubeSize,
		vector3F( 0.5f,  0.5f, -0.5f) * cubeSize,
		vector3F( 0.5f,  0.5f,  0.5f) * cubeSize,
	};
	vector3F normals[] =
	{
		vector3F(0, 1, 0),
		vector3F(0, -1, 0),
		vector3F(0, 0, 1),
		vector3F(0, 0, -1),
		vector3F(1, 0, 0),
		vector3F(-1, 0, 0),
	};

	vector3S cellOffset[] =
	{
		vector3S(0, 1, 0),
		vector3S(0, -1, 0),
		vector3S(0, 0, 1),
		vector3S(0, 0, -1),
		vector3S(1, 0, 0),
		vector3S(-1, 0, 0),
	};

	std::vector<ColoredVertex> verticesVector = std::vector<ColoredVertex>();
	std::vector<unsigned int> indicesVector;
	unsigned int indicesOffset = 0;

	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		vector3S pos = it->first;
		vector3F offset = (vector3F)pos * cubeSize;

		//Top
		if (!hasCell(pos + cellOffset[0]) && false)
		{
			verticesVector.push_back({ vertsCube[3] + offset, normals[0], outsideColor });
			verticesVector.push_back({ vertsCube[7] + offset, normals[0], outsideColor });
			verticesVector.push_back({ vertsCube[6] + offset, normals[0], outsideColor });
			verticesVector.push_back({ vertsCube[2] + offset, normals[0], outsideColor });

			//CounterClockwise ordering
			indicesVector.push_back(0 + indicesOffset);
			indicesVector.push_back(1 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(3 + indicesOffset);
			indicesVector.push_back(0 + indicesOffset);
			indicesOffset += 4;
		}

		//Bottom
		if (!hasCell(pos + cellOffset[1]))
		{
			verticesVector.push_back({ vertsCube[5] + offset, normals[1], outsideColor });
			verticesVector.push_back({ vertsCube[1] + offset, normals[1], outsideColor });
			verticesVector.push_back({ vertsCube[0] + offset, normals[1], outsideColor });
			verticesVector.push_back({ vertsCube[4] + offset, normals[1], outsideColor });

			//CounterClockwise ordering
			indicesVector.push_back(0 + indicesOffset);
			indicesVector.push_back(1 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(3 + indicesOffset);
			indicesVector.push_back(0 + indicesOffset);
			indicesOffset += 4;
		}

		//forward
		if (!hasCell(pos + cellOffset[2]))
		{
			verticesVector.push_back({ vertsCube[7] + offset, normals[2], outsideColor });
			verticesVector.push_back({ vertsCube[3] + offset, normals[2], outsideColor });
			verticesVector.push_back({ vertsCube[1] + offset, normals[2], outsideColor });
			verticesVector.push_back({ vertsCube[5] + offset, normals[2], outsideColor });

			//CounterClockwise ordering
			indicesVector.push_back(0 + indicesOffset);
			indicesVector.push_back(1 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(3 + indicesOffset);
			indicesVector.push_back(0 + indicesOffset);
			indicesOffset += 4;
		}

		//Back
		if (!hasCell(pos + cellOffset[3]))
		{
			verticesVector.push_back({ vertsCube[2] + offset, normals[3], outsideColor });
			verticesVector.push_back({ vertsCube[6] + offset, normals[3], outsideColor });
			verticesVector.push_back({ vertsCube[4] + offset, normals[3], outsideColor });
			verticesVector.push_back({ vertsCube[0] + offset, normals[3], outsideColor });

			//CounterClockwise ordering
			indicesVector.push_back(0 + indicesOffset);
			indicesVector.push_back(1 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(3 + indicesOffset);
			indicesVector.push_back(0 + indicesOffset);
			indicesOffset += 4;
		}

		//Left
		if (!hasCell(pos + cellOffset[4]))
		{
			verticesVector.push_back({ vertsCube[6] + offset, normals[4], outsideColor });
			verticesVector.push_back({ vertsCube[7] + offset, normals[4], outsideColor });
			verticesVector.push_back({ vertsCube[5] + offset, normals[4], outsideColor });
			verticesVector.push_back({ vertsCube[4] + offset, normals[4], outsideColor });

			//CounterClockwise ordering
			indicesVector.push_back(0 + indicesOffset);
			indicesVector.push_back(1 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(3 + indicesOffset);
			indicesVector.push_back(0 + indicesOffset);
			indicesOffset += 4;
		}

		//Right
		if (!hasCell(pos + cellOffset[5]))
		{
			ColoredVertex vertices[] = { 
			{ vertsCube[3] + offset, normals[5], outsideColor },
			{ vertsCube[2] + offset, normals[5], outsideColor },
			{ vertsCube[0] + offset, normals[5], outsideColor },
			{ vertsCube[1] + offset, normals[5], outsideColor } 
			};

			PushQuad(verticesVector, indicesVector, indicesOffset, vertices);
		}
	}

	if (!verticesVector.empty())
	{
		this->m_OutsideMesh = new ColoredMesh(verticesVector, indicesVector);
	}
}

void ShipEditor::updateInsideMesh()
{
	if (m_InsideMesh != 0)
	{
		delete m_InsideMesh;
		m_InsideMesh = nullptr;
	}

	vector3F vertsCube[] =
	{
		vector3F(-0.5f, -0.5f, -0.5f) * (cubeSize - 0.2f),
		vector3F(-0.5f, -0.5f, 0.5f) * (cubeSize - 0.2f),
		vector3F(-0.5f, 0.5f, -0.5f) * (cubeSize - 0.2f),
		vector3F(-0.5f, 0.5f, 0.5f) * (cubeSize - 0.2f),
		vector3F(0.5f, -0.5f, -0.5f) * (cubeSize - 0.2f),
		vector3F(0.5f, -0.5f, 0.5f) * (cubeSize - 0.2f),
		vector3F(0.5f, 0.5f, -0.5f) * (cubeSize - 0.2f),
		vector3F(0.5f, 0.5f, 0.5f) * (cubeSize - 0.2f),
	};
	vector3F normals[] =
	{
		vector3F(0, -1, 0),
		vector3F(0, 1, 0),
		vector3F(0, 0, -1),
		vector3F(0, 0, 1),
		vector3F(-1, 0, 0),
		vector3F(1, 0, 0),
	};

	std::vector<ColoredVertex> verticesVector = std::vector<ColoredVertex>();
	std::vector<unsigned int> indicesVector;
	unsigned int indicesOffset = 0;

	const float wallThickness = cubeSize - insideCubeSize;

	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		vector3S pos = it->first;
		vector3F offset = (vector3F)pos * cubeSize;


		//Ceiling 
		if (!hasCell(pos + vector3S(0, 1, 0)))
		{
			vector3F topLeft = vector3F(0.5f, 0.5f, 0.5f) * insideCubeSize;
			vector3F topRight = vector3F(-0.5f, 0.5f, 0.5f) * insideCubeSize;
			vector3F bottomRight = vector3F(-0.5f, 0.5f, -0.5f) * insideCubeSize;
			vector3F bottomLeft = vector3F(0.5f, 0.5f, -0.5f) * insideCubeSize;

			if (hasCell(pos + vector3S(0, 0, 1)) && !hasCell(pos + vector3S(1, 0, 0)))
			{
				topLeft.z += wallThickness;
				topRight.z += wallThickness;
			}

			if (hasCell(pos + vector3S(1, 0, 0)) && !hasCell(pos + vector3S(0, 0, 1)))
			{
				topLeft.x += wallThickness;
				bottomLeft.x += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 0, 1)) && hasCell(pos + vector3S(1, 0, 0)) && hasCell(pos + vector3S(1, 0, 1)))
			{
				topLeft.x += wallThickness;
				bottomLeft.x += wallThickness;
				topLeft.z += wallThickness;
				topRight.z += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 0, 1)) && hasCell(pos + vector3S(1, 0, 0)) && !hasCell(pos + vector3S(1, 0, 1)))
			{
				ColoredVertex vertices[] = {
					{ topLeft + offset + vector3F(wallThickness, 0, 0), normals[1], vector3F(0, 1, 0) },
					{ topLeft + offset, normals[1], vector3F(0, 1, 0) },
					{ bottomRight + offset, normals[1], vector3F(0, 1, 0) },
					{ bottomLeft + offset + vector3F(wallThickness, 0, 0), normals[1], vector3F(0, 1, 0) }
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices);

				ColoredVertex vertices1[] = {
					{ topLeft + offset + vector3F(0, 0, wallThickness), normals[1], vector3F(0, 1, 0) },
					{ topRight + offset + vector3F(0, 0, wallThickness), normals[1], vector3F(0, 1, 0) },
					{ bottomRight + offset, normals[1], vector3F(0, 1, 0) },
					{ topLeft + offset, normals[1], vector3F(0, 1, 0) }
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices1);
			}
			else
			{
				ColoredVertex vertices[] = {
					{ topLeft + offset, normals[1], vector3F(0, 1, 0) },
					{ topRight + offset, normals[1], vector3F(0, 1, 0) },
					{ bottomRight + offset, normals[1], vector3F(0, 1, 0) },
					{ bottomLeft + offset, normals[1], vector3F(0, 1, 0) }
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices);
			}
		}

		//Floor
		if (!hasCell(pos + vector3S(0, -1, 0)))
		{
			vector3F topLeft = vector3F(0.5f, -0.5f, 0.5f) * insideCubeSize;
			vector3F topRight = vector3F(-0.5f, -0.5f, 0.5f) * insideCubeSize;
			vector3F bottomRight = vector3F(-0.5f, -0.5f, -0.5f) * insideCubeSize;
			vector3F bottomLeft = vector3F(0.5f, -0.5f, -0.5f) * insideCubeSize;

			if (hasCell(pos + vector3S(0, 0, 1)) && !hasCell(pos + vector3S(1, 0, 0)))
			{
				topLeft.z += wallThickness;
				topRight.z += wallThickness;
			}

			if (hasCell(pos + vector3S(1, 0, 0)) && !hasCell(pos + vector3S(0, 0, 1)))
			{
				topLeft.x += wallThickness;
				bottomLeft.x += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 0, 1)) && hasCell(pos + vector3S(1, 0, 0)) && hasCell(pos + vector3S(1, 0, 1)))
			{
				topLeft.x += wallThickness;
				bottomLeft.x += wallThickness;
				topLeft.z += wallThickness;
				topRight.z += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 0, 1)) && hasCell(pos + vector3S(1, 0, 0)) && !hasCell(pos + vector3S(1, 0, 1)))
			{
				ColoredVertex vertices[] = {
					{ topLeft + offset + vector3F(wallThickness, 0, 0), normals[1], vector3F(1, 0, 1) },
					{ bottomLeft + offset + vector3F(wallThickness, 0, 0), normals[1], vector3F(1, 0, 1) },
					{ bottomRight + offset, normals[1], vector3F(1, 0, 1) },
					{ topLeft + offset, normals[1], vector3F(1, 0, 1) },
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices);

				ColoredVertex vertices1[] = {
					{ topLeft + offset + vector3F(0, 0, wallThickness), normals[1], vector3F(1, 0, 1) },
					{ topLeft + offset, normals[1], vector3F(1, 0, 1) },
					{ bottomRight + offset, normals[1], vector3F(1, 0, 1) },
					{ topRight + offset + vector3F(0, 0, wallThickness), normals[1], vector3F(1, 0, 1) },
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices1);
			}
			else
			{
				ColoredVertex vertices[] = {
					{ topLeft + offset, normals[1], vector3F(1, 0, 1) },
					{ bottomLeft + offset, normals[1], vector3F(1, 0, 1) },
					{ bottomRight + offset, normals[1], vector3F(1, 0, 1) },
					{ topRight + offset, normals[1], vector3F(1, 0, 1) },
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices);
			}
		}

		//North
		if (!hasCell(pos + vector3S(0, 0, 1)))
		{
			vector3F topLeft = vector3F(0.5f, 0.5f, 0.5f) * insideCubeSize;
			vector3F topRight = vector3F(-0.5f, 0.5f, 0.5f) * insideCubeSize;
			vector3F bottomRight = vector3F(-0.5f, -0.5f, 0.5f) * insideCubeSize;
			vector3F bottomLeft = vector3F(0.5f, -0.5f, 0.5f) * insideCubeSize;

			if (hasCell(pos + vector3S(0, 1, 0)) && !hasCell(pos + vector3S(1, 0, 0)))
			{
				topLeft.y += wallThickness;
				topRight.y += wallThickness;
			}

			if (hasCell(pos + vector3S(1, 0, 0)) && !hasCell(pos + vector3S(0, 1, 0)))
			{
				topLeft.x += wallThickness;
				bottomLeft.x += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 1, 0)) && hasCell(pos + vector3S(1, 0, 0)) && hasCell(pos + vector3S(1, 1, 0)))
			{
				topLeft.x += wallThickness;
				bottomLeft.x += wallThickness;
				topLeft.z += wallThickness;
				topRight.z += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 0, 1)) && hasCell(pos + vector3S(1, 0, 0)) && !hasCell(pos + vector3S(1, 0, 1)))
			{
				ColoredVertex vertices[] = {
					{ topLeft + offset + vector3F(wallThickness, 0, 0), normals[1], vector3F(0, 0, 1) },
					{ bottomLeft + offset + vector3F(wallThickness, 0, 0), normals[1], vector3F(0, 0, 1) },
					{ bottomRight + offset, normals[1], vector3F(0, 0, 1) },
					{ topLeft + offset, normals[1], vector3F(0, 0, 1) },
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices);

				ColoredVertex vertices1[] = {
					{ topLeft + offset + vector3F(0, wallThickness, 0), normals[1], vector3F(0, 0, 1) },
					{ topLeft + offset, normals[1], vector3F(0, 0, 1) },
					{ bottomRight + offset, normals[1], vector3F(0, 0, 1) },
					{ topRight + offset + vector3F(0, wallThickness, 0), normals[1], vector3F(0, 0, 1) },
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices1);
			}
			else
			{
				ColoredVertex vertices[] = {
					{ topLeft + offset, normals[1], vector3F(0, 0, 1) },
					{ bottomLeft + offset, normals[1], vector3F(0, 0, 1) },
					{ bottomRight + offset, normals[1], vector3F(0, 0, 1) },
					{ topRight + offset, normals[1], vector3F(0, 0, 1) },
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices);
			}
		}


		//South
		if (!hasCell(pos + vector3S(0, 0, -1)))
		{
			vector3F topLeft = vector3F(0.5f, 0.5f, -0.5f) * insideCubeSize;
			vector3F topRight = vector3F(-0.5f, 0.5f, -0.5f) * insideCubeSize;
			vector3F bottomRight = vector3F(-0.5f, -0.5f, -0.5f) * insideCubeSize;
			vector3F bottomLeft = vector3F(0.5f, -0.5f, -0.5f) * insideCubeSize;

			if (hasCell(pos + vector3S(0, 1, 0)) && !hasCell(pos + vector3S(1, 0, 0)))
			{
				topLeft.y += wallThickness;
				topRight.y += wallThickness;
			}

			if (hasCell(pos + vector3S(1, 0, 0)) && !hasCell(pos + vector3S(0, 1, 0)))
			{
				topLeft.x += wallThickness;
				bottomLeft.x += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 1, 0)) && hasCell(pos + vector3S(1, 0, 0)) && hasCell(pos + vector3S(1, 1, 0)))
			{
				topLeft.x += wallThickness;
				bottomLeft.x += wallThickness;
				topLeft.z += wallThickness;
				topRight.z += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 0, 1)) && hasCell(pos + vector3S(1, 0, 0)) && !hasCell(pos + vector3S(1, 0, 1)))
			{
				ColoredVertex vertices[] = {
					{ topLeft + offset + vector3F(wallThickness, 0, 0), normals[1], vector3F(1, 1, 0) },
					{ topLeft + offset, normals[1], vector3F(1, 1, 0) },
					{ bottomRight + offset, normals[1], vector3F(1, 1, 0) },
					{ bottomLeft + offset + vector3F(wallThickness, 0, 0), normals[1], vector3F(1, 1, 0) }
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices);

				ColoredVertex vertices1[] = {
					{ topLeft + offset + vector3F(0, wallThickness, 0), normals[1], vector3F(1, 1, 0) },
					{ topRight + offset + vector3F(0, wallThickness, 0), normals[1], vector3F(1, 1, 0) },
					{ bottomRight + offset, normals[1], vector3F(1, 1, 0) },
					{ topLeft + offset, normals[1], vector3F(1, 1, 0) }
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices1);
			}
			else
			{
				ColoredVertex vertices[] = {
					{ topLeft + offset, normals[1], vector3F(1, 1, 0) },
					{ topRight + offset, normals[1], vector3F(1, 1, 0) },
					{ bottomRight + offset, normals[1], vector3F(1, 1, 0) },
					{ bottomLeft + offset, normals[1], vector3F(1, 1, 0) }
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices);
			}
		}

		//West
		if (!hasCell(pos + vector3S(1, 0, 0)))
		{
			vector3F topLeft = vector3F(0.5f, 0.5f, -0.5f) * insideCubeSize;
			vector3F topRight = vector3F(0.5f, 0.5f, 0.5f) * insideCubeSize;
			vector3F bottomRight = vector3F(0.5f, -0.5f, 0.5f) * insideCubeSize;
			vector3F bottomLeft = vector3F(0.5f, -0.5f, -0.5f) * insideCubeSize;

			if (hasCell(pos + vector3S(0, 1, 0)) && !hasCell(pos + vector3S(1, 0, 0)))
			{
				topLeft.y += wallThickness;
				topRight.y += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 0, 1)) && !hasCell(pos + vector3S(0, 1, 0)))
			{
				topLeft.z += wallThickness;
				bottomLeft.z += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 1, 0)) && hasCell(pos + vector3S(1, 0, 0)) && hasCell(pos + vector3S(1, 1, 0)))
			{
				topLeft.x += wallThickness;
				bottomLeft.x += wallThickness;
				topLeft.z += wallThickness;
				topRight.z += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 0, 1)) && hasCell(pos + vector3S(1, 0, 0)) && !hasCell(pos + vector3S(1, 0, 1)))
			{
				ColoredVertex vertices[] = {
					{ topLeft + offset + vector3F(wallThickness, 0, 0), normals[1], vector3F(1.0f) - outsideColor },
					{ bottomLeft + offset + vector3F(wallThickness, 0, 0), normals[1], vector3F(1.0f) - outsideColor },
					{ bottomRight + offset, normals[1], vector3F(1.0f) - outsideColor },
					{ topLeft + offset, normals[1], vector3F(1.0f) - outsideColor },
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices);

				ColoredVertex vertices1[] = {
					{ topLeft + offset + vector3F(0, wallThickness, 0), normals[1], vector3F(1.0f) - outsideColor },
					{ topLeft + offset, normals[1], vector3F(1.0f) - outsideColor },
					{ bottomRight + offset, normals[1], vector3F(1.0f) - outsideColor },
					{ topRight + offset + vector3F(0, wallThickness, 0), normals[1], vector3F(1.0f) - outsideColor },
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices1);
			}
			else
			{
				ColoredVertex vertices[] = {
					{ topLeft + offset, normals[1], vector3F(1.0f) - outsideColor },
					{ bottomLeft + offset, normals[1], vector3F(1.0f) - outsideColor },
					{ bottomRight + offset, normals[1], vector3F(1.0f) - outsideColor },
					{ topRight + offset, normals[1], vector3F(1.0f) - outsideColor },
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices);
			}
		}


		//East
		if (!hasCell(pos + vector3S(0, 0, -1)) && false)
		{
			vector3F topLeft = vector3F(0.5f, 0.5f, -0.5f) * insideCubeSize;
			vector3F topRight = vector3F(-0.5f, 0.5f, -0.5f) * insideCubeSize;
			vector3F bottomRight = vector3F(-0.5f, -0.5f, -0.5f) * insideCubeSize;
			vector3F bottomLeft = vector3F(0.5f, -0.5f, -0.5f) * insideCubeSize;

			if (hasCell(pos + vector3S(0, 1, 0)) && !hasCell(pos + vector3S(1, 0, 0)))
			{
				topLeft.y += wallThickness;
				topRight.y += wallThickness;
			}

			if (hasCell(pos + vector3S(1, 0, 0)) && !hasCell(pos + vector3S(0, 1, 0)))
			{
				topLeft.x += wallThickness;
				bottomLeft.x += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 1, 0)) && hasCell(pos + vector3S(1, 0, 0)) && hasCell(pos + vector3S(1, 1, 0)))
			{
				topLeft.x += wallThickness;
				bottomLeft.x += wallThickness;
				topLeft.z += wallThickness;
				topRight.z += wallThickness;
			}

			if (hasCell(pos + vector3S(0, 0, 1)) && hasCell(pos + vector3S(1, 0, 0)) && !hasCell(pos + vector3S(1, 0, 1)))
			{
				ColoredVertex vertices[] = {
					{ topLeft + offset + vector3F(wallThickness, 0, 0), normals[1], outsideColor },
					{ topLeft + offset, normals[1], outsideColor },
					{ bottomRight + offset, normals[1], outsideColor },
					{ bottomLeft + offset + vector3F(wallThickness, 0, 0), normals[1], outsideColor }
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices);

				ColoredVertex vertices1[] = {
					{ topLeft + offset + vector3F(0, wallThickness, 0), normals[1], outsideColor },
					{ topRight + offset + vector3F(0, wallThickness, 0), normals[1], outsideColor },
					{ bottomRight + offset, normals[1], outsideColor },
					{ topLeft + offset, normals[1], outsideColor }
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices1);
			}
			else
			{
				ColoredVertex vertices[] = {
					{ topLeft + offset, normals[1], outsideColor },
					{ topRight + offset, normals[1], outsideColor },
					{ bottomRight + offset, normals[1], outsideColor },
					{ bottomLeft + offset, normals[1], outsideColor }
				};
				PushQuad(verticesVector, indicesVector, indicesOffset, vertices);
			}
		}

	}

	if (!verticesVector.empty())
	{
		this->m_InsideMesh = new ColoredMesh(verticesVector, indicesVector);
	}
}

void ShipEditor::PushQuad(std::vector<ColoredVertex>& verticesVector, std::vector<unsigned int>& indicesVector, unsigned int& indicesOffset, ColoredVertex verticesToAdd[4])
{
	verticesVector.push_back(verticesToAdd[0]);
	verticesVector.push_back(verticesToAdd[1]);
	verticesVector.push_back(verticesToAdd[2]);
	verticesVector.push_back(verticesToAdd[3]);

	//Clockwise ordering
	indicesVector.push_back(0 + indicesOffset);
	indicesVector.push_back(1 + indicesOffset);
	indicesVector.push_back(2 + indicesOffset);
	indicesVector.push_back(0 + indicesOffset);
	indicesVector.push_back(2 + indicesOffset);
	indicesVector.push_back(3 + indicesOffset);
	indicesOffset += 4;
}


ShipEditor::~ShipEditor()
{

}