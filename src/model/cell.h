#ifndef __CELL_H__
#define __CELL_H__
#include "terrain.h"
#include "item.h"

class Star;

/**
 * @brief The Cell class
 * This class represents a cell in the game board.
 */
class Cell
{
public:
	enum Visibility
	{
		none,	/* don't display */
		exists, /* display as "unknown" */
		visible /* display as item name */
	};

private:
	Visibility visibility;	/* item visibility */
	const Item *item;		/* pointer to item */
	const Terrain *terrain; /* pointer to terrain */
	const Star *star;		/* pointer to star */

public:
	Cell();

	/* trivial setters */
	void setItem(const Item *item);
	void setTerrain(const Terrain *terrain);
	void setStar(const Star *star);
	void setVisibility(Visibility visibility);

	/**
	 * @brief render terrain to console
	 * @param current true if this cell is the current cell (for dirty magic only)
	 */
	void renderTerrain(bool current);

	/**
	 * @brief render item or star to console
	 * @param current true if this cell is the current cell (for dirty magic only)
	 * @note star's render precedences are higher than item's
	 */
	void renderItem(bool current);

	/**
	 * @brief see if the cat will trigger something in the cell, if so, the item will be removed after trigger; will also display victory message if the cat wins
	 * @param cat the cat
	 * @return true if cat has triggered something
	 */
	bool trigger(Cat &cat);

	/**
	 * @brief check if the cell contains enemy
	 * @return true if the cell contains enemy
	 */
	bool isEnemy() const;

	/**
	 * @brief the cat tries to hunt prey in the cell
	 * @param cat the cat
	 * @return true if the cat has hunted prey, whether it is a success or not
	 */
	bool hunt(Cat &cat);
	const Terrain *getTerrain() const;
};

#endif