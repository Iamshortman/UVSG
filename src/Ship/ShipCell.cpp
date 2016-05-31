#include "ShipCell.hpp"

#include "Rendering/ObjLoader.hpp"
#include <jsoncons/json.hpp>
using jsoncons::json;
using jsoncons::pretty_print;

Node::Node(vector3S position, int direction)
{
	m_position = position;
	m_direction = direction;
}

bool Node::operator== (const Node& other)
{
	return (m_position == other.m_position) && (m_direction == other.m_direction);
}

ShipCell::ShipCell(Mesh* mesh, Mesh* cursorMesh, double mass, std::vector<Node> nodes, AABB aabb)
{
	m_mesh = mesh;
	m_cursorMesh = cursorMesh;
	m_mass = mass;
	m_nodes = nodes;
	m_aabb = aabb;
}

ShipCell::ShipCell(string jsonFile)
{
	json file = json::parse_file(jsonFile);
	json cell = file["ShipCell"];

	//Load Cell from file
	m_mass = cell["mass"].as<double>();
	
	string mesh_loc = cell["mesh_loc"].as<string>();
	string mesh = cell["mesh"].as<string>();
	string mesh_cursor = cell["mesh_cursor"].as<string>();

	if (mesh != "" && mesh_loc != "")
	{
		m_mesh = loadMaterialMeshFromFile(mesh_loc, mesh);
	}

	if (mesh_cursor != "" && mesh_loc != "")
	{
		m_cursorMesh = loadMaterialMeshFromFile(mesh_loc, mesh_cursor);
	}

	json nodes = cell["nodes"];
	for (int i = 0; i < nodes.size(); i++)
	{
		int x = nodes[i]["position"][0].as<int>();
		int y = nodes[i]["position"][1].as<int>();
		int z = nodes[i]["position"][2].as<int>();

		int direction = nodes[i]["direction"].as<int>();

		m_nodes.push_back(Node(vector3S(x, y, z), direction));
	}

	vector<double> min_pos = cell["aabbMin"].as<vector<double>>();
	vector<double> max_pos = cell["aabbMax"].as<vector<double>>();
	m_aabb = AABB(vector3D(min_pos[0], min_pos[1], min_pos[2]), vector3D(max_pos[0], max_pos[1], max_pos[2]));
}

ShipCell::~ShipCell()
{
	delete m_mesh;
	delete m_cursorMesh;
}

double ShipCell::getCellMass()
{
	return m_mass;
}

Mesh* ShipCell::getMesh()
{
	return m_mesh;
}

Mesh*ShipCell::getCursorMesh()
{
	return m_cursorMesh;
}

std::vector<Node> ShipCell::getNodePoints()
{
	return m_nodes;
}

AABB ShipCell::getAABB()
{
	return m_aabb;
}