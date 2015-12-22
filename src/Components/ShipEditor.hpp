#ifndef SHIPEDITOR_HPP
#define SHIPEDITOR_HPP

#include <unordered_map>

#include "glmInclude.hpp"
#include "Rendering/ColoredMesh.hpp"
#include "Renderable.hpp"
#include "Rendering\Camera.hpp"
#include "Rendering\TexturePool.hpp"

struct Quad
{
	Quad(){};
	Quad(vector3F a, vector3F b, vector3F c, vector3F d)
	{
		m_a = a;
		m_b = b;
		m_c = c;
		m_d = d;
	};
	vector3F m_a;
	vector3F m_b;
	vector3F m_c;
	vector3F m_d;
};

struct insideCubeFace
{
	/*
	[00][10][20]
	[01][11][21]
	[02][12][22]
	*/
	Quad m_faces[3][3];

	//m_Checks is a cell offest to check if the quad in the corresponding m_faces location should be drawn.
	//The Mesh gen function will deal with this logic.
	vector3S m_Checks[3][3];

	vector3F m_normal;
};

struct ShipCell
{

};

struct KeyFuncs
{
	size_t operator()(const vector3S& k)const
	{
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
	}

	bool operator()(const vector3S& a, const vector3S& b)const
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

typedef std::unordered_map<vector3S, int, KeyFuncs, KeyFuncs> Ship_Map;

class ShipEditor
{
public:
	ShipEditor();
	virtual ~ShipEditor();

	void Update();
	void TempRender(Camera& camera, TexturePool& texturePool);
	void addCell(vector3S pos);
	void removeCell(vector3S pos);
	bool hasCell(vector3S pos);
	void updateOutsideMesh();
	void updateInsideMesh();

	Ship_Map m_shipCells;
	ColoredMesh* m_OutsideMesh;
	ColoredMesh* m_InsideMesh;
	vector3F outsideColor = vector3F(155, 155, 255) / 255.0f;
	float cubeSize = 2.2f;
	float insideCubeSize = 2.0f;
	ShaderProgram* shader;

	vector3S m_cursorPos;

	Model* tempModel;

	bool needsUpdate = false;

	void PushQuad(std::vector<ColoredVertex>& verticesVector, std::vector<unsigned int>& indicesVector, unsigned int& indicesOffset, ColoredVertex verticesToAdd[4]);
	void PushQuad(std::vector<ColoredVertex>& verticesVector, std::vector<unsigned int>& indicesVector, unsigned int& indicesOffset, Quad points, vector3F normal, vector3F color, vector3F offset);

private:
};

#endif //SHIPEDITOR_HPP