#include "ShipCellDictionary.hpp"

ShipCellDictionary::ShipCellDictionary()
{
	m_shipCells = hash_map<string, ShipCell*>();
}

ShipCellDictionary::~ShipCellDictionary()
{
	//Delete all Cells
	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		delete it->second;
	}
}

void ShipCellDictionary::loadAllCellsFromFolder(string folder)
{
	//Loads Ship Cells from .json files
	_finddata_t data;
	intptr_t ff = _findfirst((folder + "*.json").c_str(), &data);

	if (ff != -1)
	{
		int res = 0;
		while (res != -1)
		{
			string fileName = data.name;
			printf("Loading Ship Cell from: %s \n", (folder + fileName).c_str());
			ShipCell* cell = new ShipCell(folder + fileName);

			if (m_shipCells.find(cell->getCellId()) != m_shipCells.end())
			{
				perror(("ShipCell " + cell->getCellId() + "is already loaded!!!\n").c_str());
				exit(1);
			}

			m_shipCells[cell->getCellId()] = cell;

			string category;

			for (int i = 0; i < cell->m_categories.size(); i++)
			{
				category = cell->m_categories[i];

				if (m_cellCategories.find(category) == m_cellCategories.end())
				{
					m_cellCategories[category] = vector<ShipCell*>();
				}
				m_cellCategories[category].push_back(cell);
			}

			res = _findnext(ff, &data);
		}
		_findclose(ff);
	}

}

ShipCell* ShipCellDictionary::getCell(string name)
{
	if (m_shipCells.find(name) != m_shipCells.end())
	{
		return m_shipCells[name];
	}

	return nullptr;
}

vector<ShipCell*> ShipCellDictionary::getCategory(string categoryName)
{
	if (m_cellCategories.find(categoryName) != m_cellCategories.end())
	{
		return m_cellCategories[categoryName];
	}

	//If it doesnt exist, return empty vector
	return vector<ShipCell*>();
}