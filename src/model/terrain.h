#ifndef __TERRAIN_H__
#define __TERRAIN_H__
#include "../utils/multistring.h"
#include <vector>
#include <random>

class Cell;

/**
 * @brief The Terrain class
 * control terrain name and cells with the specific terrain
 */
class Terrain
{
private:
	MultiString name;		   /* multi-language name */
	bool visible;			   /* useless, always true */
	std::vector<Cell *> cells; /* cells with the specific terrain */

public:
	Terrain(MultiString name, bool visible);
	MultiString getName() const;

	/* add cell to the terrain */
	void addCell(Cell *cell);

	/**
	 * @brief get and remove a random cell from all cells with the specific terrain
	 * @param cell pointer to the cell
	 * @return true if success, false if failed
	 */
	bool randomCell(Cell *&cell);
};

#endif