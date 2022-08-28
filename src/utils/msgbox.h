#ifndef __MSGBOX_H__
#define __MSGBOX_H__
#include "../model/board.h"
#include "../model/cat.h"
#include "../controller/game.h"

/**
 * @brief The MsgBox class
 * helper class to display messages
 */
class MsgBox
{
private:
	static const int ROWS = 25;
	static const int COLS = 80;
	static Board *board;
	static Cat *cat;
	static Game *game;
	static bool fallback;

public:
	static void initBoard(Board *b);
	static void initCat(Cat *c);
	static void initGame(Game *g);
	static void initFallback(bool f);

	/**
	 * @brief create a message box with OK button
	 * @param msg message to display
	 * @param msg2 second message line (optional)
	 */
	static void create(const char *msg, const char *msg2 = nullptr);

	/**
	 * @brief create a message box with two buttons
	 * @param msg message to display
	 * @param option1 first button text
	 * @param option2 second button text
	 * @return 1 if first button is pressed, 2 if second button is pressed
	 */
	static int create(const char *msg, const char *option1, const char *option2);
};

#endif