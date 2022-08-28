#include "msgbox.h"
#include "utils.h"
#ifdef _WIN32
#include "../pdcurses/curses.h"
#else
#include "../ncurses/ncurses.h"
#endif
#include <cstring>

Board *MsgBox::board = nullptr;
Cat *MsgBox::cat = nullptr;
Game *MsgBox::game = nullptr;
bool MsgBox::fallback = false;

void MsgBox::initBoard(Board *b)
{
	board = b;
}

void MsgBox::initCat(Cat *c)
{
	cat = c;
}

void MsgBox::initGame(Game *g)
{
	game = g;
}

void MsgBox::initFallback(bool f)
{
	fallback = f;
}

void MsgBox::create(const char *msg, const char *msg2)
{
	int width = strLen(msg) + 6;
	int height = 5;
	if (msg2)
	{
		width = std::max<int>(width, strLen(msg2) + 6);
		height++;
	}
	int starty = (ROWS - height) / 2;
	int startx = (COLS - width) / 2;
	WINDOW *win = newwin(height, width, starty, startx);
	if (fallback)
		wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	else
		box(win, 0, 0); // draw with UTF-8 characters
	wrefresh(win);
	mvwprintw(win, 1, (width - strLen(msg)) / 2, msg);
	if (msg2)
		mvwprintw(win, 2, (width - strLen(msg2)) / 2, msg2);
	wattron(win, A_REVERSE); // highlight the OK button
	mvwprintw(win, height - 2, width / 2 - 4, paddingMid("OK", false).c_str());
	wattroff(win, A_REVERSE);
	wrefresh(win);
	for (int ch = getch(); ch != '\n' && ch != '\r'; ch = getch())
		;												  // wait for user to press enter
	wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '); // clear border
	wrefresh(win);
	delwin(win);
	// render destoryed board and cat again
#ifdef _WIN32
	clear();
	game->renderStatus();
#endif
	board->renderScreen(fallback);
	cat->renderAttributes(fallback);
	refresh();
}

int MsgBox::create(const char *msg, const char *option1, const char *option2)
{
	int width = strLen(msg) + 6;
	int height = 5;
	int starty = (ROWS - height) / 2;
	int startx = (COLS - width) / 2;
	WINDOW *win = newwin(height, width, starty, startx);
	if (fallback)
		wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	else
		box(win, 0, 0);
	wrefresh(win);
	int option = 1;
	for (;;)
	{
		mvwprintw(win, 1, 3, msg);
		if (option == 1)
			wattron(win, A_REVERSE);
		mvwprintw(win, height - 2, width / 8, paddingMid(option1, false).c_str());
		wattroff(win, A_REVERSE);
		if (option == 2)
			wattron(win, A_REVERSE);
		mvwprintw(win, height - 2, width - width / 8 - 8, paddingMid(option2, false).c_str());
		wattroff(win, A_REVERSE);
		wrefresh(win);
		int c = getch();
		if (c == '\n' || c == '\r')
			break;
		else if (c == KEY_LEFT)
			option = 1;
		else if (c == KEY_RIGHT)
			option = 2;
	}
	wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(win);
	delwin(win);
#ifdef _WIN32
	clear();
	game->renderStatus();
#endif
	board->renderScreen(fallback);
	cat->renderAttributes(fallback);
	refresh();
	return option;
}