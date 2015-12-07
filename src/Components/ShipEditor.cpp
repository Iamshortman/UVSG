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
		this->updateMesh();
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
		this->m_OutsideMesh->draw();
	}
	shader->deactivateProgram();

	tempModel->shader->setActiveProgram();
	tempModel->shader->setUniform("MVP", MVP);
	tempModel->shader->setUniform("normalMatrix", normalMatrix);
	texturePool.bindTexture("res/arrow-up.png");
	Transform localtransform;
	localtransform.setPos(m_cursorPos);
	tempModel->shader->setUniform("localOffset", localtransform.getModleMatrix());
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

void ShipEditor::updateMesh()
{
	if (m_OutsideMesh != 0)
	{
		delete m_OutsideMesh;
		m_OutsideMesh = nullptr;
	}

	vector3F vertsCube[] =
	{
		vector3F(-0.5f, -0.5f, -0.5f),
		vector3F(-0.5f, -0.5f, 0.5f),
		vector3F(-0.5f, 0.5f, -0.5f),
		vector3F(-0.5f, 0.5f, 0.5f),
		vector3F(0.5f, -0.5f, -0.5f),
		vector3F(0.5f, -0.5f, 0.5f),
		vector3F(0.5f, 0.5f, -0.5f),
		vector3F(0.5f, 0.5f, 0.5f),
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
		vector3F offset = (vector3F)pos;

		//Top
		if (!hasCell(pos + cellOffset[0]))
		{
			verticesVector.push_back({ vertsCube[3] + offset, normals[0], outsideColor });
			verticesVector.push_back({ vertsCube[7] + offset, normals[0], outsideColor });
			verticesVector.push_back({ vertsCube[6] + offset, normals[0], outsideColor });
			verticesVector.push_back({ vertsCube[2] + offset, normals[0], outsideColor });

			//Clockwise ordering
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

			//Clockwise ordering
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

			//Clockwise ordering
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

			//Clockwise ordering
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

			//Clockwise ordering
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
			verticesVector.push_back({ vertsCube[3] + offset, normals[5], outsideColor });
			verticesVector.push_back({ vertsCube[2] + offset, normals[5], outsideColor });
			verticesVector.push_back({ vertsCube[0] + offset, normals[5], outsideColor });
			verticesVector.push_back({ vertsCube[1] + offset, normals[5], outsideColor });

			//Clockwise ordering
			indicesVector.push_back(0 + indicesOffset);
			indicesVector.push_back(1 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(3 + indicesOffset);
			indicesVector.push_back(0 + indicesOffset);
			indicesOffset += 4;
		}
	}

	if (!verticesVector.empty())
	{
		this->m_OutsideMesh = new ColoredMesh(verticesVector, indicesVector);
	}
}


ShipEditor::~ShipEditor()
{

}