#include "game.h"
#include "../utils/msgbox.h"
#include "../utils/utils.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <random>
#include <cctype>
#ifdef _WIN32
#include "../pdcurses/curses.h"
#else
#include "../ncurses/ncurses.h"
#endif

std::minstd_rand Game::rng(time(nullptr));

std::minstd_rand &Game::getRng()
{
	return rng;
}

Game::Game(int argc, char *argv[]) : fallback(false)
{
#ifdef _WIN32
	MsgBox::initGame(this);
	system("chcp 65001>nul"); // set console code page to UTF-8
#endif
	MultiString::setLanguage(MultiString::en);
	parseArguments(argc, argv);
	std::setlocale(LC_ALL, ""); // set locale to UTF-8
	initscr();					// initialize curses
	raw();						// no line buffering, receive all characters
	keypad(stdscr, TRUE);		// enable keypad & function keys
	start_color();				// enable colors
	readAttributes();
}

Game::~Game()
{
	endwin(); // end curses
}

void Game::run()
{
	board.placeTerrain();
	board.placeItems();
	board.placeStars();
	board.clearScreen();
	remainStep = cat.generateStep();
	MsgBox::initFallback(fallback);
	for (;;)
	{
#ifdef _WIN32
		clear(); // clear screen under Windows for compatibility
#endif
		board.renderScreen(fallback);
		cat.renderAttributes(fallback);
		renderStatus();
		if (!process())
			break;
	}
}

void Game::parseArguments(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg == "-h" || arg == "--help" || arg == "-?")
		{
			std::cout << MultiString({"Usage: " + std::string(argv[0]) + " [OPTION]...", "使用方法: " + std::string(argv[0]) + " [选项]..."}) << std::endl;
			std::cout << "--help, -h, -?: " << MultiString({"show this help message and exit", "显示帮助信息并退出"}) << std::endl;
			std::cout << "--version, -v: " << MultiString({"show version information and exit", "显示版本信息并退出"}) << std::endl;
			std::cout << "--language, -l: " << MultiString({"set language", "设置语言"}) << std::endl;
			std::cout << "--invincible, -i: " << MultiString({"You won't die", "你不会死"}) << std::endl;
			std::cout << "--health, --hunger, --thirst: " << MultiString({"Modify values, will raise max value if >= 16", "修改值，如果大于等于16，则会提高最大值"}) << std::endl;
			std::cout << "--strength, --speed, --skill, --defense: " << MultiString({"Modify values", "修改值"}) << std::endl;
			std::cout << "--stepmax: " << MultiString({"Specify how many steps you can walk before decrease status values", "指定你可以走多少步之后会减少状态值"}) << std::endl;
			std::cout << "--no-water-penalty: " << MultiString({"You won't lose health in water even if you are not RiverClan", "你不会在水中损失生命值，即使你不是河族"}) << std::endl;
			std::cout << "--diagonal: " << MultiString({"You can move diagonally even if you are not ShadowClan", "你可以在对角线上移动，即使你不是影族"}) << std::endl;
			std::cout << "--cat-value-max: " << MultiString({"Specify the maximum value of cat's status", "指定猫的状态值的最大值"}) << std::endl;
			std::cout << "--fallback: " << MultiString({"Use fallback graphic mode", "使用简易图形模式"}) << std::endl;
			exit(0);
		}
		else if (arg == "-l" || arg == "--language")
		{
			if (i + 1 < argc)
			{
				std::string lang = argv[++i];
				if (lang == "zh")
					MultiString::setLanguage(MultiString::zh);
				else if (lang == "en")
					MultiString::setLanguage(MultiString::en);
				else
				{
					std::cout << MultiString({"Unknown language: " + lang, "未知语言: " + lang}) << std::endl;
					exit(1);
				}
			}
			else
			{
				std::cout << MultiString({"Missing language argument", "缺少语言参数"}) << std::endl;
				exit(1);
			}
		}
		else if (arg == "-v" || arg == "--version")
		{
			std::cout << MultiString({"TNPQuest version 0.1", "TNPQuest 版本 0.1"}) << std::endl;
			exit(0);
		}
		else if (arg == "-i" || arg == "--invincible")
			cat.setInvincible();
		else if (arg == "--health")
		{
			if (i + 1 < argc)
				cat.setHealth(std::atoi(argv[++i]));
			else
			{
				std::cout << MultiString({"Missing health argument", "缺少生命值参数"}) << std::endl;
				exit(1);
			}
		}
		else if (arg == "--hunger")
		{
			if (i + 1 < argc)
				cat.setHunger(std::atoi(argv[++i]));
			else
			{
				std::cout << MultiString({"Missing hunger argument", "缺少饥饿值参数"}) << std::endl;
				exit(1);
			}
		}
		else if (arg == "--thirst")
		{
			if (i + 1 < argc)
				cat.setThirst(std::atoi(argv[++i]));
			else
			{
				std::cout << MultiString({"Missing thirst argument", "缺少口渴值参数"}) << std::endl;
				exit(1);
			}
		}
		else if (arg == "--stepmax")
		{
			if (i + 1 < argc)
				cat.setMaxStep(std::atoi(argv[++i]));
			else
			{
				std::cout << MultiString({"Missing stepmax argument", "缺少stepmax参数"}) << std::endl;
				exit(1);
			}
		}
		else if (arg == "--strength")
		{
			if (i + 1 < argc)
				cat.setStrength(std::atoi(argv[++i]));
			else
			{
				std::cout << MultiString({"Missing strength argument", "缺少力量参数"}) << std::endl;
				exit(1);
			}
		}
		else if (arg == "--speed")
		{
			if (i + 1 < argc)
				cat.setSpeed(std::atoi(argv[++i]));
			else
			{
				std::cout << MultiString({"Missing speed argument", "缺少速度参数"}) << std::endl;
				exit(1);
			}
		}
		else if (arg == "--skill")
		{
			if (i + 1 < argc)
				cat.setSkill(std::atoi(argv[++i]));
			else
			{
				std::cout << MultiString({"Missing skill argument", "缺少技巧参数"}) << std::endl;
				exit(1);
			}
		}
		else if (arg == "--defense")
		{
			if (i + 1 < argc)
				cat.setDefense(std::atoi(argv[++i]));
			else
			{
				std::cout << MultiString({"Missing defense argument", "缺少防御参数"}) << std::endl;
				exit(1);
			}
		}
		else if (arg == "--cat-value-max")
		{
			if (i + 1 < argc)
				cat.setMaxVal(std::atoi(argv[++i]));
			else
			{
				std::cout << MultiString({"Missing cat-value-max argument", "缺少cat-value-max参数"}) << std::endl;
				exit(1);
			}
		}
		else if (arg == "--no-water-penalty")
			cat.setWaterproof();
		else if (arg == "--diagonal")
			cat.setDiagonal();
		else if (arg == "--fallback")
			fallback = true;
		else
		{
			std::cout << MultiString({"Unknown argument: " + arg, "未知参数: " + arg}) << std::endl;
			std::cout << MultiString({"Try `" + std::string(argv[0]) + " --help' for more information.", "请尝试 `" + std::string(argv[0]) + " --help' 查看更多信息。"}) << std::endl;
			exit(1);
		}
	}
}

void Game::readAttributes()
{
	cat.randomAttributes();
	printw(MultiString({"ThunderClan: +2 Strength\n", "雷族: +2 力量\n"}));
	printw(MultiString({"WindClan: +2 Speed\n", "风族: +2 速度\n"}));
	printw(MultiString({"RiverClan: No water movement penalty\n", "河族: 在水中移动无惩罚\n"}));
	printw(MultiString({"ShadowClan: Can attack and move diagonally\n", "影族: 可以斜着攻击和移动\n"}));
	for (;;)
	{
		printw(MultiString({"Choose your Clan (t/w/r/s): ", "选择你的族群 (t/w/r/s 代表 雷族/风族/河族/影族): "}));
		char c = getch();
		if (cat.setClan(tolower(c)))
			break;
	}
	printw(MultiString({"\nBefore you start your quest, you need to receive your apprentice name.\n", "\n在开始你的任务之前，你需要接受你的学徒名字。\n"}));
	char s[32]; // potential buffer overflow
	for (;;)
	{
		printw(MultiString({"Your first name: ", "你的名字（请输入英文）："}));
		getstr(s);
		if (cat.setName(s))
			break;
	}
	printw(MultiString({"Your apprentice name is " + cat.getApprenticeName() + ", press any key to continue.\n", "你的学徒名字是" + cat.getApprenticeName() + "，按任意键继续。\n"}));
	noecho();	 // disable echo
	curs_set(0); // disable cursor
	getch();
}

bool Game::process()
{
	if (remainStep)
	{
		mvprintw(Board::rows * 3 + 8, 0, MultiString({"Press Enter to stop", "按回车停止"}));
		int ch = getch();
		if (ch == 'q')
			return false;
		if (ch == '\n' || ch == '\r') // enter to stop
		{
			remainStep = 0;
			cat.loseVal(); // lose cat value if reach max step
			return true;
		}
		board.backupCoordinates();
		if (!board.move(ch, cat))
			return true;
		if (cat.getDiagonal())
		{
			halfdelay(1); // receive another key within 0.1 seconds
			ch = getch(); // if no key is pressed, returns ERR
			if (ch == 'q')
				return false;
			raw(); // disable halfdelay
			if (ch != ERR && !board.move(ch, cat))
			{
				board.restoreCoordinates(); // rollback
				return true;
			}
		}
		mvprintw(Board::rows * 3 + 8, 0, std::string(Board::boxWidth, ' ').c_str());
		mvprintw(Board::rows * 3 + 9, 0, std::string(Board::boxWidth, ' ').c_str());
		refresh();
		if (board.trigger(cat)) // the cat trigger something
		{
			remainStep = cat.generateStep(); // regenerate step
			cat.loseVal();
			return true;
		}
		if (!--remainStep) // use up step
			cat.loseVal();
	}
	else
	{
		if (!doAction()) // game over
			return false;
		remainStep = cat.generateStep();
	}
	return true;
}

void Game::renderStatus()
{
	if (fallback)
		mvprintw(Board::rows * 3 + 5, 0, MultiString({"|   Clan   |   Name   |   Map    |   Step   |   Star   |", "|   族群   |   名字   |   地图   |   步数   |   星星   |"}));
	else
		mvprintw(Board::rows * 3 + 5, 0, MultiString({"│   Clan   │   Name   │   Map    │   Step   │   Star   │", "│   族群   │   名字   │   地图   │   步数   │   星星   │"}));
	mvprintw(Board::rows * 3 + 6, 0, fallback ? "|" : "│");
	switch (cat.getClan())
	{
	case Cat::Clan::thunder:
		printw(paddingMid(std::string(MultiString({"ThundrClan", "雷族"})), false, 10).c_str());
		break;
	case Cat::Clan::wind:
		printw(paddingMid(std::string(MultiString({"WindClan", "风族"})), false, 10).c_str());
		break;
	case Cat::Clan::river:
		printw(paddingMid(std::string(MultiString({"RiverClan", "河族"})), false, 10).c_str());
		break;
	case Cat::Clan::shadow:
		printw(paddingMid(std::string(MultiString({"ShadowClan", "影族"})), false, 10).c_str());
		break;
	default:
		printw(" Unknown ");
		break;
	}
	printw(fallback ? ("|" + paddingMid(cat.getApprenticeName(), false, 10) + "| %2d / 13  |    %2d    |  %d / 4   |").c_str() : ("│" + paddingMid(cat.getApprenticeName(), false, 10) + "│ %2d / 13  │    %2d    │  %d / 4   │").c_str(), board.getScreen() + 1, remainStep, cat.getStars());
	mvprintw(Board::rows * 3 + 7, 0, fallback ? "+" : "└");
	for (int i = 1; i < 55; i++)
		if (fallback)
			printw(i % 11 ? "-" : "+");
		else
			printw(i % 11 ? "─" : "┴");
	printw(fallback ? "+" : "┘");
}

bool Game::doAction()
{
	mvprintw(Board::rows * 3 + 8, 0, std::string(Board::boxWidth, ' ').c_str());
	MultiString choices[6] = {MultiString({"Battle", "战斗"}), MultiString({"Check Scent", "检查气味"}), MultiString({"Hunt", "捕猎"}), MultiString({"Explore", "探索"}), MultiString({"Rest", "休息"}), MultiString({"Drink", "喝水"})};
	int choice = 0;
	for (;;)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int idx = i * 3 + j;
				if (idx == choice)
					attron(A_REVERSE); // highlight current choice
				mvprintw(Board::rows * 3 + 8 + i, j * 15, paddingMid(std::string(choices[idx]), fallback, 12).c_str());
				attroff(A_REVERSE);
			}
			/*
			if (fallback && i == choice / 3)
			{
				mvprintw(Board::rows * 3 + 8 + i, 0, "|");
				for (int j = 0; j < choice % 3; j++)
				{
					int idx = i * 3 + j;
					printw(paddingMid(std::string(choices[idx]), false).c_str());
					printw("|");
				}
			}
			*/
		}
		int ch = getch();
		if (ch == 'q')
			return false;
		if (ch == '\n' || ch == '\r')
			break;
		if (ch == KEY_UP)
		{
			if (choice >= 3)
				choice -= 3;
		}
		else if (ch == KEY_DOWN)
		{
			if (choice < 3)
				choice += 3;
		}
		else if (ch == KEY_LEFT)
		{
			if (choice > 0)
				choice--;
		}
		else if (ch == KEY_RIGHT)
		{
			if (choice < 5)
				choice++;
		}
		mvprintw(Board::rows * 3 + 8, 0, std::string(Board::boxWidth, ' ').c_str());
		mvprintw(Board::rows * 3 + 9, 0, std::string(Board::boxWidth, ' ').c_str());
		refresh();
	}
	mvprintw(Board::rows * 3 + 8, 0, std::string(Board::boxWidth, ' ').c_str());
	mvprintw(Board::rows * 3 + 9, 0, std::string(Board::boxWidth, ' ').c_str());
	refresh();
	switch (choice)
	{
	case 0:
		board.battle(cat, fallback);
		break;
	case 1:
		board.clearScreen(Cell::Visibility::exists);
		break;
	case 2:
		board.hunt(cat, fallback);
		break;
	case 3:
		board.explore(cat, fallback);
		break;
	case 4:
		cat.rest();
		break;
	case 5:
		if (!board.getWater())
			MsgBox::create(MultiString({"No water here", "这里没有水"}));
		else
			cat.drink();
		break;
	}
	mvprintw(Board::rows * 3 + 8, 0, std::string(Board::boxWidth, ' ').c_str());
	mvprintw(Board::rows * 3 + 9, 0, std::string(Board::boxWidth, ' ').c_str());
	refresh();
	return true;
}