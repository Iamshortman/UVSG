#include "ShipCell.hpp"

#include "Rendering/ObjLoader.hpp"

#include <jsoncons/json.hpp>
using jsoncons::json;
using jsoncons::pretty_print;

#include <Physics/BoxCollisionShape.hpp>
#include <Physics/MeshCollisionShape.hpp>

Node::Node(vector3B position, int direction)
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
	json cell = json::parse_file(jsonFile);

	m_cellId = cell["cell_ID"].as<string>();

	//Load Cell from file
	m_mass = cell["mass"].as<double>();
	
	if (cell.has_member("mesh") && cell.has_member("mesh_loc"))
	{
		string mesh = cell["mesh"].as<string>();
		string mesh_loc = cell["mesh_loc"].as<string>();
		m_mesh = loadMaterialMeshFromFile(mesh_loc, mesh);
	}

	if (cell.has_member("mesh_cursor") && cell.has_member("mesh_loc"))
	{
		string mesh = cell["mesh_cursor"].as<string>();
		string mesh_loc = cell["mesh_loc"].as<string>();
		m_cursorMesh = loadMaterialMeshFromFile(mesh_loc, mesh);
	}

	if (cell.has_member("mesh_interior") && cell.has_member("mesh_loc"))
	{
		string mesh = cell["mesh_interior"].as<string>();
		string mesh_loc = cell["mesh_loc"].as<string>();
		m_interiorMesh = loadMaterialMeshFromFile(mesh_loc, mesh);
	}

	if (cell.has_member("nodes"))
	{
		json nodes = cell["nodes"];
		for (int i = 0; i < nodes.size(); i++)
		{
			vector<int> pos = nodes[i]["position"].as<vector<int>>();
			int direction = nodes[i]["direction"].as<int>();

			m_nodes.push_back(Node(vector3B(pos[0], pos[1], pos[2]), direction));
		}
	}

	if (cell.has_member("internalNode"))
	{
		json nodes = cell["nodes"];
		for (int i = 0; i < nodes.size(); i++)
		{
			vector<int> pos = nodes[i]["position"].as<vector<int>>();
			int direction = nodes[i]["direction"].as<int>();

			m_internalNode.push_back(Node(vector3B(pos[0], pos[1], pos[2]), direction));
		}
	}

	vector<double> min_pos = cell["aabbMin"].as<vector<double>>();
	vector<double> max_pos = cell["aabbMax"].as<vector<double>>();
	m_aabb = AABB(vector3D(min_pos[0], min_pos[1], min_pos[2]), vector3D(max_pos[0], max_pos[1], max_pos[2]));

	if (cell.has_member("physics_box"))
	{
		json physcis = cell["physics_box"];
		vector<double> size = physcis["halfExtends"].as<vector<double>>();
		vector<double> offset = physcis["offset"].as<vector<double>>();
		shapeOffset = vector3D(offset[0], offset[1], offset[2]);
		shape = new BoxCollisionShape(vector3D(size[0], size[1], size[2]));
	}
	else if (cell.has_member("physics_mesh"))
	{
		json physcis = cell["physics_mesh"]; 

		string mesh_loc = physcis["mesh_loc"].as<string>();
		string mesh = physcis["mesh"].as<string>();

		shape = new MeshCollisionShape(mesh_loc, mesh);

		vector<double> offset = physcis["offset"].as<vector<double>>();
		shapeOffset = vector3D(offset[0], offset[1], offset[2]);
	}


	if (cell.has_member("cockpit"))
	{
		json seats = cell["cockpit"];
		for (int i = 0; i < seats.size(); i++)
		{
			vector<double> pos = seats[i]["seat_position"].as<vector<double>>();
			int direction = seats[i]["seat_direction"].as<int>();

			m_seats.push_back(CockpitSeat(vector3D(pos[0], pos[1], pos[2]), direction));
		}
	}

	if (cell.has_member("categories"))
	{
		m_categories = cell["categories"].as<vector<string>>();
	}

}

ShipCell::~ShipCell()
{
	delete shape;
	delete m_mesh;
	delete m_cursorMesh;
}

string ShipCell::getCellId()
{
	return m_cellId;
}

double ShipCell::getCellMass()
{
	return m_mass;
}

Mesh* ShipCell::getMesh()
{
	return m_mesh;
}

Mesh* ShipCell::getCursorMesh()
{
	return m_cursorMesh;
}

Mesh* ShipCell::getInteriorMesh()
{
	return m_interiorMesh;
}

vector<Node> ShipCell::getNodePoints()
{
	return m_nodes;
}

vector<Node> ShipCell::getInternalNodePoints()
{
	return m_internalNode;
}

AABB ShipCell::getAABB()
{
	return m_aabb;
}