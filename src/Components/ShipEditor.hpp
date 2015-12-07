#ifndef SHIPEDITOR_HPP
#define SHIPEDITOR_HPP

#include <unordered_map>

#include "glmInclude.hpp"
#include "Rendering/ColoredMesh.hpp"
#include "Renderable.hpp"
#include "Rendering\Camera.hpp"
#include "Rendering\TexturePool.hpp"

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
	void updateMesh();

	Ship_Map m_shipCells;
	ColoredMesh* m_OutsideMesh;
	ColoredMesh* m_InsideMesh;
	vector3F outsideColor = vector3F(155, 155, 255) / 255.0f;
	ShaderProgram* shader;

	vector3S m_cursorPos;

	Model* tempModel;

	bool needsUpdate = false;
};

#endif //SHIPEDITOR_HPP