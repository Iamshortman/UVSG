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

	static Uint8 lastState_LSHIFT = 0;
	if (state[SDL_SCANCODE_LSHIFT] != 0 && lastState_LSHIFT == 0)
	{
		m_cursorPos.y += 1;
	}
	lastState_LSHIFT = state[SDL_SCANCODE_LSHIFT];

	static Uint8 lastState_LCTRL = 0;
	if (state[SDL_SCANCODE_LCTRL] != 0 && lastState_LCTRL == 0)
	{
		m_cursorPos.y += -1;
	}
	lastState_LCTRL = state[SDL_SCANCODE_LCTRL];

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
	localTransform.setPos((vector3D) m_cursorPos);
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

void GenInsideMesh(float outsideCubeSize, float insideCubeSize, std::vector<ColoredVertex>& verticesVector, std::vector<unsigned int>& indicesVector, unsigned int& indicesOffset);

void ShipEditor::updateInsideMesh()
{
	if (m_InsideMesh != 0)
	{
		delete m_InsideMesh;
		m_InsideMesh = nullptr;
	}

	//from center to outside wall
	float outside = cubeSize / 2.0f;

	//from center to inside wall
	float inside = insideCubeSize / 2.0f;

	//-------------------------------------------------------------------------
	insideCubeFace celeiling;
	celeiling.m_normal = vector3F(0, -1, 0);
	celeiling.m_faces[0][0] = Quad(vector3F(-outside, inside, outside), vector3F(-outside, inside, inside), vector3F(-inside, inside, inside), vector3F(-inside, inside, outside));
	celeiling.m_faces[1][0] = Quad(vector3F(-inside, inside, outside), vector3F(-inside, inside, inside), vector3F(inside, inside, inside), vector3F(inside, inside, outside));
	celeiling.m_faces[2][0] = Quad(vector3F(inside, inside, outside), vector3F(inside, inside, inside), vector3F(outside, inside, inside), vector3F(outside, inside, outside));

	celeiling.m_faces[0][1] = Quad(vector3F(-outside, inside, inside), vector3F(-outside, inside, -inside), vector3F(-inside, inside, -inside), vector3F(-inside, inside, inside));
	celeiling.m_faces[1][1] = Quad(vector3F(-inside, inside, inside), vector3F(-inside, inside, -inside), vector3F(inside, inside, -inside), vector3F(inside, inside, inside));
	celeiling.m_faces[2][1] = Quad(vector3F(inside, inside, inside), vector3F(inside, inside, -inside), vector3F(outside, inside, -inside), vector3F(outside, inside, inside));

	celeiling.m_faces[0][2] = Quad(vector3F(-outside, inside, -inside), vector3F(-outside, inside, -outside), vector3F(-inside, inside, -outside), vector3F(-inside, inside, -inside));
	celeiling.m_faces[1][2] = Quad(vector3F(-inside, inside, -inside), vector3F(-inside, inside, -outside), vector3F(inside, inside, -outside), vector3F(inside, inside, -inside));
	celeiling.m_faces[2][2] = Quad(vector3F(inside, inside, -inside), vector3F(inside, inside, -outside), vector3F(outside, inside, -outside), vector3F(outside, inside, -inside));

	celeiling.m_Checks[0][0] = vector3S(-1, 0, 1); //Right Forward
	celeiling.m_Checks[1][0] = vector3S(0, 0, 1); //Forward
	celeiling.m_Checks[2][0] = vector3S(1, 0, 1); //Left Forward

	celeiling.m_Checks[0][1] = vector3S(-1, 0, 0); //Right
	celeiling.m_Checks[1][1] = vector3S(0, 1, 0); //Up
	celeiling.m_Checks[2][1] = vector3S(1, 0, 0); //Left

	celeiling.m_Checks[0][2] = vector3S(-1, 0, -1); //Right Back
	celeiling.m_Checks[1][2] = vector3S(0, 0, -1); //Back
	celeiling.m_Checks[2][2] = vector3S(1, 0, -1); //Left Back
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	insideCubeFace floor;
	floor.m_normal = vector3F(0, 1, 0);
	floor.m_faces[0][0] = Quad(vector3F(outside, -inside, outside), vector3F(outside, -inside, inside), vector3F(inside, -inside, inside), vector3F(inside, -inside, outside));
	floor.m_faces[1][0] = Quad(vector3F(inside, -inside, outside), vector3F(inside, -inside, inside), vector3F(-inside, -inside, inside), vector3F(-inside, -inside, outside));
	floor.m_faces[2][0] = Quad(vector3F(-inside, -inside, outside), vector3F(-inside, -inside, inside), vector3F(-outside, -inside, inside), vector3F(-outside, -inside, outside));

	floor.m_faces[0][1] = Quad(vector3F(outside, -inside, inside), vector3F(outside, -inside, -inside), vector3F(inside, -inside, -inside), vector3F(inside, -inside, inside));
	floor.m_faces[1][1] = Quad(vector3F(inside, -inside, inside), vector3F(inside, -inside, -inside), vector3F(-inside, -inside, -inside), vector3F(-inside, -inside, inside));
	floor.m_faces[2][1] = Quad(vector3F(-inside, -inside, inside), vector3F(-inside, -inside, -inside), vector3F(-outside, -inside, -inside), vector3F(-outside, -inside, inside));

	floor.m_faces[0][2] = Quad(vector3F(outside, -inside, -inside), vector3F(outside, -inside, -outside), vector3F(inside, -inside, -outside), vector3F(inside, -inside, -inside));
	floor.m_faces[1][2] = Quad(vector3F(inside, -inside, -inside), vector3F(inside, -inside, -outside), vector3F(-inside, -inside, -outside), vector3F(-inside, -inside, -inside));
	floor.m_faces[2][2] = Quad(vector3F(-inside, -inside, -inside), vector3F(-inside, -inside, -outside), vector3F(-outside, -inside, -outside), vector3F(-outside, -inside, -inside));

	floor.m_Checks[0][0] = vector3S(1, 0, 1); //Left Forward
	floor.m_Checks[1][0] = vector3S(0, 0, 1); //Foward
	floor.m_Checks[2][0] = vector3S(-1, 0, 1); //Right Forward

	floor.m_Checks[0][1] = vector3S(1, 0, 0); //Left
	floor.m_Checks[1][1] = vector3S(0, -1, 0); //Down
	floor.m_Checks[2][1] = vector3S(-1, 0, 0); //Right

	floor.m_Checks[0][2] = vector3S(1, 0, -1); //Left Back
	floor.m_Checks[1][2] = vector3S(0, 0, -1); //Back
	floor.m_Checks[2][2] = vector3S(-1, 0, -1); //Right Back
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	insideCubeFace northWall;
	northWall.m_normal = vector3F(0, 0, -1);
	northWall.m_faces[0][0] = Quad(vector3F(outside, outside, inside), vector3F(outside, inside, inside), vector3F(inside, inside, inside), vector3F(inside, outside, inside));
	northWall.m_faces[1][0] = Quad(vector3F(inside, outside, inside), vector3F(inside, inside, inside), vector3F(-inside, inside, inside), vector3F(-inside, outside, inside));
	northWall.m_faces[2][0] = Quad(vector3F(-inside, outside, inside), vector3F(-inside, inside, inside), vector3F(-outside, inside, inside), vector3F(-outside, outside, inside));

	northWall.m_faces[0][1] = Quad(vector3F(outside, inside, inside), vector3F(outside, -inside, inside), vector3F(inside, -inside, inside), vector3F(inside, inside, inside));
	northWall.m_faces[1][1] = Quad(vector3F(inside, inside, inside), vector3F(inside, -inside, inside), vector3F(-inside, -inside, inside), vector3F(-inside, inside, inside));
	northWall.m_faces[2][1] = Quad(vector3F(-inside, inside, inside), vector3F(-inside, -inside, inside), vector3F(-outside, -inside, inside), vector3F(-outside, inside, inside));

	northWall.m_faces[0][2] = Quad(vector3F(outside, -inside, inside), vector3F(outside, -outside, inside), vector3F(inside, -outside, inside), vector3F(inside, -inside, inside));
	northWall.m_faces[1][2] = Quad(vector3F(inside, -inside, inside), vector3F(inside, -outside, inside), vector3F(-inside, -outside, inside), vector3F(-inside, -inside, inside));
	northWall.m_faces[2][2] = Quad(vector3F(-inside, -inside, inside), vector3F(-inside, -outside, inside), vector3F(-outside, -outside, inside), vector3F(-outside, -inside, inside));

	northWall.m_Checks[0][0] = vector3S(1, 1, 0); //Up Left
	northWall.m_Checks[1][0] = vector3S(0, 1, 0); //Up
	northWall.m_Checks[2][0] = vector3S(-1, 1, 0); //Up Right

	northWall.m_Checks[0][1] = vector3S(1, 0, 0); //Left
	northWall.m_Checks[1][1] = vector3S(0, 0, 1); //North
	northWall.m_Checks[2][1] = vector3S(-1, 0, 0); //Right

	northWall.m_Checks[0][2] = vector3S(1, -1, 0); //Down Left
	northWall.m_Checks[1][2] = vector3S(0, -1, 0); //Down
	northWall.m_Checks[2][2] = vector3S(-1, -1, 0); //Down Right
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	insideCubeFace westWall;
	westWall.m_normal = vector3F(-1, 0, 0);
	westWall.m_faces[0][0] = Quad(vector3F(inside, outside, -outside), vector3F(inside, inside, -outside), vector3F(inside, inside, -inside), vector3F(inside, outside, -inside));
	westWall.m_faces[1][0] = Quad(vector3F(inside, outside, -inside), vector3F(inside, inside, -inside), vector3F(inside, inside, inside), vector3F(inside, outside, inside));
	westWall.m_faces[2][0] = Quad(vector3F(inside, outside, inside), vector3F(inside, inside, inside), vector3F(inside, inside, outside), vector3F(inside, outside, outside));

	westWall.m_faces[0][1] = Quad(vector3F(inside, inside, -outside), vector3F(inside, -inside, -outside), vector3F(inside, -inside, -inside), vector3F(inside, inside, -inside));
	westWall.m_faces[1][1] = Quad(vector3F(inside, inside, -inside), vector3F(inside, -inside, -inside), vector3F(inside, -inside, inside), vector3F(inside, inside, inside));
	westWall.m_faces[2][1] = Quad(vector3F(inside, inside, inside), vector3F(inside, -inside, inside), vector3F(inside, -inside, outside), vector3F(inside, inside, outside));

	westWall.m_faces[0][2] = Quad(vector3F(inside, -inside, -outside), vector3F(inside, -outside, -outside), vector3F(inside, -outside, -inside), vector3F(inside, -inside, -inside));
	westWall.m_faces[1][2] = Quad(vector3F(inside, -inside, -inside), vector3F(inside, -outside, -inside), vector3F(inside, -outside, inside), vector3F(inside, -inside, inside));
	westWall.m_faces[2][2] = Quad(vector3F(inside, -inside, inside), vector3F(inside, -outside, inside), vector3F(inside, -outside, outside), vector3F(inside, -inside, outside));

	westWall.m_Checks[0][0] = vector3S(0, 1, -1); //Up Back
	westWall.m_Checks[1][0] = vector3S(0, 1, 0); //Up
	westWall.m_Checks[2][0] = vector3S(0, 1, 1); //Up Forward

	westWall.m_Checks[0][1] = vector3S(0, 0, -1); //Back
	westWall.m_Checks[1][1] = vector3S(1, 0, 0); //West
	westWall.m_Checks[2][1] = vector3S(0, 0, 1); //Forward

	westWall.m_Checks[0][2] = vector3S(0, -1, -1); //Down Back
	westWall.m_Checks[1][2] = vector3S(0, -1, 0); //Down
	westWall.m_Checks[2][2] = vector3S(0, -1, 1); //Down Forward
	//-------------------------------------------------------------------------

	std::vector<insideCubeFace> faces = { celeiling, floor, northWall, westWall };

	std::vector<ColoredVertex> verticesVector = std::vector<ColoredVertex>();
	std::vector<unsigned int> indicesVector;
	unsigned int indicesOffset = 0;

	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		vector3S pos = it->first;
		vector3F offset = (vector3F)pos * cubeSize;

		for (int i = 0; i < faces.size(); i++)
		{
			insideCubeFace face = faces[i];

			if (!hasCell(pos + face.m_Checks[1][1]))
			{
				PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[1][1], face.m_normal, vector3F(1, 0, 1), offset);

				if (hasCell(pos + face.m_Checks[1][0]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[1][0], face.m_normal, vector3F(1, 0, 1), offset);
				}
				if (hasCell(pos + face.m_Checks[1][2]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[1][2], face.m_normal, vector3F(1, 0, 1), offset);
				}
				if (hasCell(pos + face.m_Checks[0][1]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[0][1], face.m_normal, vector3F(1, 0, 1), offset);
				}

				if (hasCell(pos + face.m_Checks[2][1]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[2][1], face.m_normal, vector3F(1, 0, 1), offset);
				}

				//Top Left Corner
				if (hasCell(pos + face.m_Checks[0][0]) && hasCell(pos + face.m_Checks[1][0]) && hasCell(pos + face.m_Checks[0][1]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[0][0], face.m_normal, vector3F(1, 0, 1), offset);
				}

				//Top Right Corner
				if (hasCell(pos + face.m_Checks[2][0]) && hasCell(pos + face.m_Checks[1][0]) && hasCell(pos + face.m_Checks[2][1]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[2][0], face.m_normal, vector3F(1, 0, 1), offset);
				}

				//Bottom Left Corner
				if (hasCell(pos + face.m_Checks[0][2]) && hasCell(pos + face.m_Checks[1][2]) && hasCell(pos + face.m_Checks[0][1]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[0][2], face.m_normal, vector3F(1, 0, 1), offset);
				}

				//Bottom Right Corner
				if (hasCell(pos + face.m_Checks[2][2]) && hasCell(pos + face.m_Checks[1][2]) && hasCell(pos + face.m_Checks[2][1]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[2][2], face.m_normal, vector3F(1, 0, 1), offset);
				}

				//Top Left Corner
				if (hasCell(pos + face.m_Checks[0][1]) && hasCell(pos + face.m_Checks[0][0]) && hasCell(pos + face.m_Checks[1][0]) && hasCell(pos + face.m_Checks[0][1] + face.m_Checks[1][1]) && hasCell(pos + face.m_Checks[0][0] + face.m_Checks[1][1]) && hasCell(pos + face.m_Checks[1][0] + face.m_Checks[1][1]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[0][0], face.m_normal, vector3F(1, 1, 1), offset);
				}

			}
			else
			{
				if (hasCell(pos + face.m_Checks[1][0]) && !hasCell(pos + face.m_Checks[1][0] + face.m_Checks[1][1]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[1][0], face.m_normal, vector3F(0, 0, 1), offset);
				}

				if (hasCell(pos + face.m_Checks[1][2]) && !hasCell(pos + face.m_Checks[1][2] + face.m_Checks[1][1]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[1][2], face.m_normal, vector3F(0, 0, 1), offset);
				}

				if (hasCell(pos + face.m_Checks[0][1]) && !hasCell(pos + face.m_Checks[0][1] + face.m_Checks[1][1]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[0][1], face.m_normal, vector3F(0, 0, 1), offset);
				}

				if (hasCell(pos + face.m_Checks[2][1]) && !hasCell(pos + face.m_Checks[2][1] + face.m_Checks[1][1]))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[2][1], face.m_normal, vector3F(0, 0, 1), offset);
				}


				//Top Left Corner
				if (hasCell(pos + face.m_Checks[0][0]) && hasCell(pos + face.m_Checks[1][0]) && hasCell(pos + face.m_Checks[0][1]) && (!hasCell(pos + face.m_Checks[1][0] + face.m_Checks[1][1]) || !hasCell(pos + face.m_Checks[0][1] + face.m_Checks[1][1])))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[0][0], face.m_normal, vector3F(1, 0, 1), offset);
				}

				//Top Right Corner
				if (hasCell(pos + face.m_Checks[2][0]) && hasCell(pos + face.m_Checks[1][0]) && hasCell(pos + face.m_Checks[2][1]) && (!hasCell(pos + face.m_Checks[1][0] + face.m_Checks[1][1]) || !hasCell(pos + face.m_Checks[2][1] + face.m_Checks[1][1])))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[2][0], face.m_normal, vector3F(1, 0, 1), offset);
				}

				//Bottom Left Corner
				if (hasCell(pos + face.m_Checks[0][2]) && hasCell(pos + face.m_Checks[1][2]) && hasCell(pos + face.m_Checks[0][1]) && (!hasCell(pos + face.m_Checks[1][2] + face.m_Checks[1][1]) || !hasCell(pos + face.m_Checks[0][1] + face.m_Checks[1][1])))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[0][2], face.m_normal, vector3F(1, 0, 1), offset);
				}

				//Bottom Right Corner
				if (hasCell(pos + face.m_Checks[2][2]) && hasCell(pos + face.m_Checks[1][2]) && hasCell(pos + face.m_Checks[2][1]) && (!hasCell(pos + face.m_Checks[1][2] + face.m_Checks[1][1]) || !hasCell(pos + face.m_Checks[2][1] + face.m_Checks[1][1])))
				{
					PushQuad(verticesVector, indicesVector, indicesOffset, face.m_faces[2][2], face.m_normal, vector3F(1, 0, 1), offset);
				}
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

void ShipEditor::PushQuad(std::vector<ColoredVertex>& verticesVector, std::vector<unsigned int>& indicesVector, unsigned int& indicesOffset, Quad points, vector3F normal, vector3F color, vector3F offset)
{
	verticesVector.push_back({ points.m_a + offset, normal, color });
	verticesVector.push_back({ points.m_b + offset, normal, color });
	verticesVector.push_back({ points.m_c + offset, normal, color });
	verticesVector.push_back({ points.m_d + offset, normal, color });
	indicesVector.push_back(0 + indicesOffset);
	indicesVector.push_back(1 + indicesOffset);
	indicesVector.push_back(2 + indicesOffset);
	indicesVector.push_back(2 + indicesOffset);
	indicesVector.push_back(3 + indicesOffset);
	indicesVector.push_back(0 + indicesOffset);
	indicesOffset += 4;
}

ShipEditor::~ShipEditor()
{

}

void GenInsideMesh(float outsideCubeSize, float insideCubeSize, std::vector<ColoredVertex>& verticesVector, std::vector<unsigned int>& indicesVector, unsigned int& indicesOffset)
{
	//from center to outside wall
	float outside = outsideCubeSize / 2.0f;

	//from center to inside wall
	float inside = insideCubeSize / 2.0f;

	//-------------------------------------------------------------------------
	insideCubeFace floor;
	floor.m_faces[0][0] = Quad(vector3F(outside, -inside, outside), vector3F(outside, -inside, inside), vector3F(inside, -inside, inside), vector3F(inside, -inside, outside));
	floor.m_faces[1][0] = Quad(vector3F(inside, -inside, outside), vector3F(inside, -inside, inside), vector3F(-inside, -inside, inside), vector3F(-inside, -inside, outside));
	floor.m_faces[2][0] = Quad(vector3F(-inside, -inside, outside), vector3F(-inside, -inside, inside), vector3F(-outside, -inside, inside), vector3F(-outside, -inside, outside));

	floor.m_faces[0][1] = Quad(vector3F(outside, -inside, inside), vector3F(outside, -inside, -inside), vector3F(inside, -inside, -inside), vector3F(inside, -inside, inside));
	floor.m_faces[1][1] = Quad(vector3F(inside, -inside, inside), vector3F(inside, -inside, -inside), vector3F(-inside, -inside, -inside), vector3F(-inside, -inside, inside));
	floor.m_faces[2][1] = Quad(vector3F(-inside, -inside, inside), vector3F(-inside, -inside, -inside), vector3F(-outside, -inside, -inside), vector3F(-outside, -inside, inside));

	floor.m_faces[0][2] = Quad(vector3F(outside, -inside, -inside), vector3F(outside, -inside, -outside), vector3F(inside, -inside, -outside), vector3F(inside, -inside, -inside));
	floor.m_faces[1][2] = Quad(vector3F(inside, -inside, -inside), vector3F(inside, -inside, -outside), vector3F(-inside, -inside, -outside), vector3F(-inside, -inside, -inside));
	floor.m_faces[2][2] = Quad(vector3F(-inside, -inside, -inside), vector3F(-inside, -inside, -outside), vector3F(-outside, -inside, -outside), vector3F(-outside, -inside, -inside));

	floor.m_Checks[0][0] = vector3S(1, 0, 1); //Left Forward
	floor.m_Checks[1][0] = vector3S(0, 0, 1); //Foward
	floor.m_Checks[2][0] = vector3S(-1, 0, 1); //Right Forward

	floor.m_Checks[0][1] = vector3S(1, 0, 0); //Left
	floor.m_Checks[1][1] = vector3S(0, -1, 0); //Down
	floor.m_Checks[2][1] = vector3S(-1, 0, 0); //Right

	floor.m_Checks[0][2] = vector3S(1, 0, -1); //Left Back
	floor.m_Checks[1][2] = vector3S(0, 0, -1); //Back
	floor.m_Checks[2][2] = vector3S(-1, 0, -1); //Right Back

	//-------------------------------------------------------------------------

	for (int x = 0; x < 3; x += 2)
	{
		for (int y = 0; y < 3; y +=2)
		{
			verticesVector.push_back({ floor.m_faces[x][y].m_a, vector3F(0, 1, 0), vector3F(1, 0, 1) });
			verticesVector.push_back({ floor.m_faces[x][y].m_b, vector3F(0, 1, 0), vector3F(1, 0, 1) });
			verticesVector.push_back({ floor.m_faces[x][y].m_c, vector3F(0, 1, 0), vector3F(1, 0, 1) });
			verticesVector.push_back({ floor.m_faces[x][y].m_d, vector3F(0, 1, 0), vector3F(1, 0, 1) });
			indicesVector.push_back(0 + indicesOffset);
			indicesVector.push_back(1 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(2 + indicesOffset);
			indicesVector.push_back(3 + indicesOffset);
			indicesVector.push_back(0 + indicesOffset);
			indicesOffset += 4;
		}
	}

}
