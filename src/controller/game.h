#ifndef __GAME_H__
#define __GAME_H__
#include "../model/board.h"
#include "../utils/multistring.h"
#include "../model/cat.h"
#include <random>
#include <string>

/**
 * @brief The Game class
 * control top level of the game
 */
class Game
{
private:
	static std::minstd_rand rng; /* global random number generator */
	Board board;				 /* board of the game */
	Cat cat;					 /* player of the game */
	bool fallback;				 /* if true, be compatible with Windows */
	int remainStep;				 /* steps left in a round */

public:
	/* initialize the game with command line arguments */
	Game(int argc, char *argv[]);
	~Game();

	/* start the game */
	void run();

	/* get global random number generator */
	static std::minstd_rand &getRng();

	/* render global status */
	void renderStatus();

private:
	/* parse command line arguments */
	void parseArguments(int argc, char *argv[]);

	/* prompt player to choose cat attributes */
	void readAttributes();

	/**
	 * @brief main game loop step, process walking, triggering and actions
	 * @return false if game is over
	 */
	bool process();

	/**
	 * @brief process battle, check scent, hunt, explore, rest, and drink
	 * (may need to choose opponent)
	 * @return false if game is over
	 */
	bool doAction();
};

#endif