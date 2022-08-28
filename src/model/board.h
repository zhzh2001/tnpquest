#ifndef __BOARD_H__
#define __BOARD_H__
#include "cell.h"
#include "prey.h"
#include "injury.h"
#include "benefit.h"
#include "defense.h"
#include "enemy.h"
#include "star.h"
#include "terrain.h"
#include <vector>

/**
 * @brief The Board class
 * control the board of the game
 */
class Board
{
public:
	static const int rows = 5;		 /* rows of the board */
	static const int cols = 8;		 /* columns of the board */
	static const int boxWidth = 80;	 /* width of the board (in characters) */
	static const int boxHeight = 25; /* height of the board (in lines) */

private:
	static const int screens = 13;							  /* number of screens in game */
	static const int totalScreens = 40;						  /* all possible screens */
	static const std::vector<Prey> preys;					  /* all possible preys */
	static const std::vector<Injury> injuries;				  /* all possible injuries */
	static const std::vector<Benefit> benefits;				  /* all possible benefits */
	static const std::vector<DefenseAction> defenseActions;	  /* all possible defense actions */
	static std::vector<Enemy> enemies;						  /* all possible enemies */
	static const std::vector<Star> stars;					  /* all possible stars */
	static std::vector<Terrain> terrains;					  /* all possible terrains */
	static const std::vector<std::vector<int>> boardTerrains; /* terrains in each cell */
	std::vector<Cell *> enemyCells;							  /* cells containing enemies */
	Cell cells[screens][rows][cols];						  /* cells in the board */
	int nowScreen;											  /* current screen */
	int nowRow;												  /* current row */
	int nowCol;												  /* current column */
	int backupScreen, backupRow, backupCol;					  /* backup screen, row, column */
	int targetRow, targetCol;								  /* opponent row, column */

	/**
	 * @brief ask the player to choose a target cell
	 * result is stored in targetRow and targetCol
	 * @param diagonal if true, allow diagonal movement
	 * @param fallback if true, be compatible with Windows
	 */
	void chooseTarget(bool diagonal, bool fallback);

	/**
	 * @brief set targetRow and targetCol according to the given direction
	 * @param ch the direction
	 */
	void moveTarget(int ch);

public:
	Board();

	/**
	 * @brief render the board
	 * @param fallback if true, be compatible with Windows
	 */
	void renderScreen(bool fallback);

	/**
	 * @brief place vector of item on the board
	 * if the item is an enemy, it will be placed in enemyCells
	 * @param items items to be placed
	 */
	template <class T>
	void placeItems(const std::vector<T> &items);

	/* place necessary items on the board */
	void placeItems();

	/* random screens in game, set terrain in each cell */
	void placeTerrain();

	/* place vitals on the board */
	void placeStars();

	/**
	 * @brief set visibility of all cells in current screen
	 * @param visibility desired visibility
	 */
	void clearScreen(Cell::Visibility visibility = Cell::Visibility::none);

	/**
	 * @brief the cat tries to walk in certain direction
	 * @param ch the direction
	 * @param cat the cat
	 * @return true if cat walks successfully
	 */
	bool move(int ch, Cat &cat);

	/* backup current screen, row, column */
	void backupCoordinates();

	/* restore backup screen, row, column */
	void restoreCoordinates();

	/**
	 * see if the cat will trigger something in the cell
	 * @param cat the cat
	 * @return true if cat has triggered something
	 */
	bool trigger(Cat &cat);
	int getScreen() const;

	/**
	 * @brief the cat tries to battle with the enemy in target cell
	 * @param cat the cat
	 * @param fallback if true, be compatible with Windows
	 */
	void battle(Cat &cat, bool fallback);

	/**
	 * @brief the cat tries to hunt prey in target cell
	 * @param cat the cat
	 * @param fallback if true, be compatible with Windows
	 */
	void hunt(Cat &cat, bool fallback);

	/**
	 * @brief the cat tries to explore the target cell
	 * @param cat the cat
	 * @param fallback if true, be compatible with Windows
	 */
	void explore(Cat &cat, bool fallback);

	/**
	 * @brief see if there is water in current cell
	 * @return true if there is water
	 */
	bool getWater() const;
};

#endif