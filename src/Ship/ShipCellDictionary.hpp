#ifndef SHIPCELLDICTIONARY_HPP
#define SHIPCELLDICTIONARY_HPP
//This class will load/store all the ship cells in the game

#include <hash_map>
#include <vector>
#include <iostream>
#include <io.h>

#include "ShipCell.hpp"

using std::string;
using std::hash_map;
using std::vector;

class ShipCellDictionary
{
public:
	ShipCellDictionary();
	~ShipCellDictionary();

	void loadAllCellsFromFolder(string folder);
	ShipCell* getCell(string name);
	vector<ShipCell*> getCategory(string categoryName);

private:
	hash_map<string, ShipCell*> m_shipCells;
	hash_map<string, vector<ShipCell*>> m_cellCategories;
};


#endif //SHIPCELLDICTIONARY_HPP