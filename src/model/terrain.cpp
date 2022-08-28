#include "terrain.h"
#include "../controller/game.h"
#include "../utils/utils.h"

Terrain::Terrain(MultiString name, bool visible) : name(name), visible(visible) {}

MultiString Terrain::getName() const
{
	return name;
}

void Terrain::addCell(Cell *cell)
{
	cells.push_back(cell);
}

bool Terrain::randomCell(Cell *&cell)
{
	if (cells.size() == 0)
		return false;
	int idx = std::uniform_int_distribution<>(0, cells.size() - 1)(Game::getRng());
	// remove by swapping with the last element
	cell = cells[idx];
	cells[idx] = cells.back();
	cells.pop_back();
	return true;
}