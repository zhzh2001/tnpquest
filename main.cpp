#include "main.h"
#include "data.h"

int main(int argc, char *argv[])
{
	if (argc>1&&(std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help" || std::string(argv[1]) == "-?"))
	{
		std::cout << "--invincible, -i: You won't die\n";
		std::cout << "--health, --hunger, --thirst: Modify values, will raise catValMax if >= 16\n";
		std::cout << "--strength, --speed, --skill, --defense: Modify values\n";
		std::cout << "--stepmax: Specify how many steps you can walk before decrease status values\n";
		std::cout << "--no-water-penelty: You won't lose health in water even if you are not RiverClan\n";
		std::cout << "--diagonal: You can move diagonally even if you are not ShadowClan\n";
		std::cout << "--cat-val-max: Modify catValMax\n";
		return 0;
	}
	printver();
	pregame();
	processParams(argc, argv);
	generateBoard();
	nowCol = dice(0, boardCols - 1);
	generateStep();
	for (;;)
	{
		clearScreen();
		drawBoard();
		std::cout << "Health: " << catHealth << "/" << catValMax << "||Hunger: " << catHunger << "/" << catValMax << "||Thirst: " << catThirst << "/" << catValMax << "||Strength: " << catStrength << "+" << strengthBonus << "||Speed: " << catSpeed << "+" << speedBonus << std::endl;
		std::cout << "Defense: " << catDefense << "||Skill: " << catSkill << "||Stars: " << starFound << "/" << starCount << "||Status: " << (remainStep ? "Walking" : battling ? "Battling" : "Stopped") << "||Clan: " << (catClan == thunder ? "ThunderClan" : catClan == shadow ? "ShadowClan" : catClan == wind ? "WindClan" : "RiverClan") << "||Map: " << std::to_string(nowBoard + 1) << "/" << std::to_string(boardCount) << std::endl;
		std::cout << "< " << msg;
		std::string cmd;
		std::cout << "> " << std::flush;
		std::string tmp;
		getline(std::cin, tmp);
		if (tmp != "")
			cmd = tmp;
		msg = "";
		process(cmd);
	}
	return 0;
}

/**
 * print program version to console
**/
void printver()
{
	std::cout << "Warriors The New Prophecy Quest Game Mimicked CLI Version " << version << std::endl;
}

/**
 * generate attributes
 * query catClan
 * set bonus
**/
void pregame()
{
	catHealth = catVal(gen);
	catHunger = catVal(gen);
	catThirst = catVal(gen);
	catStrength = catVal(gen);
	catDefense = catVal(gen);
	catSpeed = catVal(gen);
	catSkill = catVal(gen);
	do
	{
		std::cout << "ThunderClan: +2 Strength\n";
		std::cout << "WindClan: +2 Speed\n";
		std::cout << "RiverClan: No water movement penalty(aka. -1 Health per step)\n";
		std::cout << "ShadowClan: Can attack and move diagonally\n";
		std::cout << "Choose your Clan(t/w/r/s): " << std::flush;
		char c;
		std::cin >> c;
		switch (c)
		{
		case 't':
			catClan = thunder;
			strengthBonus = 2;
			break;
		case 'w':
			catClan = wind;
			speedBonus = 2;
			break;
		case 'r':
			catClan = river;
			waterp = false;
			break;
		case 's':
			catClan = shadow;
			diagonal = true;
			break;
		default:
			catClan = undefined;
			std::cout << "Unrecognizable Clan" << std::endl;
		}
	} while (catClan == undefined);
	std::cout << "Before you start your quest, you need to receive you apprentice name.\n";
	std::cout << "Your first name: " << std::flush;
	std::string s;
	std::cin >> s;
	catName = std::tolower(s[0]) - 'a';
	std::cout << "Your name is " << prefixes[catName] << "paw, press Enter to continue";
	std::string tmp;
	getline(std::cin, tmp);
	getline(std::cin, tmp);
}

/**
 * generate board terrains
 * place all stuff & star
**/
void generateBoard()
{
	int p[boardTotal + 1];
	for (int i = 0; i < boardTotal; i++)
		p[i] = i;
	shuffle(p, p + boardTotal, gen);
	for (int i = 0; i < boardCount - 1; i++)
	{
		boards[i].id = p[i];
		int cellID = 0;
		for (int x = 0; x < boardRows; x++)
			for (int y = 0; y < boardCols; y++)
			{
				boards[i].matrix[x][y].terrainID = boardTerrains[boards[i].id].types[cellID++];
				if (x && x < boardRows - 1)
					terrainCells[boards[i].matrix[x][y].terrainID].push_back({i, x, y});
			}
	}
	placeStuff(to_stuffs(preys), cell_t::Prey);
	placeStuff(to_stuffs(injuries), cell_t::Injury);
	placeStuff(to_stuffs(benefits), cell_t::Benefit);
	placeStuff(to_stuffs(defenseActions), cell_t::DefenseAction);
	placeStuff(to_stuffs(enemies), cell_t::Enemy);
	placeStar();
	doSPJ();
}

/**
 * clear screen according to platform
 * currently support Windows & *nix
**/
void clearScreen()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

/**
 * place stuff & set stuffArray
 * @stuffs stuff to place
 * @count count of stuff
 * @type type of stuff
**/
void placeStuff(const std::vector<stuff> &stuffs, cell_t type)
{
	for (int i = 0; i < stuffs.size(); i++)
	{
		int rarity = stuffs[i].counts[dice(0, stuffs[i].counts.size() - 1)];
		while (rarity--)
		{
			int place = stuffs[i].places[dice(0, stuffs[i].places.size() - 1)];
			if (terrainCells[place].empty())
				continue;
			int idx = dice(0, terrainCells[place].size() - 1);
			cellPosition pos = terrainCells[place][idx];
			terrainCells[place][idx] = terrainCells[place].back();
			terrainCells[place].pop_back();
			boards[pos.boardID].matrix[pos.row][pos.col].type = type;
			boards[pos.boardID].matrix[pos.row][pos.col].stuffID = i;
		}
	}
	stuffArray[type] = stuffs;
}

/**
 * place star(need)
**/
void placeStar()
{
	std::vector<cellPosition> cellEnemies;
	for (int i = 0; i < boardCount; i++)
		for (int x = 0; x < boardRows; x++)
			for (int y = 0; y < boardCols; y++)
				if (boards[i].matrix[x][y].type == cell_t::Enemy)
					cellEnemies.push_back({i, x, y});
	for (int i = 1; i <= starCount; i++)
	{
		int idx = std::uniform_int_distribution<>(0, cellEnemies.size() - 1)(gen);
		boards[cellEnemies[idx].boardID].matrix[cellEnemies[idx].row][cellEnemies[idx].col].starID = i;
		cellEnemies[idx] = cellEnemies.back();
		cellEnemies.pop_back();
	}
}

/**
 * place something special as the flash version does
**/
void doSPJ()
{
	boards[0].matrix[1][2].starID = 2;
	boards[0].matrix[1][2].type = cell_t::Enemy;
	boards[0].matrix[1][2].stuffID = 6;
}

/**
 * clear current board
 * a.k.a. set all visibility to sth
**/
void clearBoard()
{
	for (int x = 0; x < boardRows; x++)
		for (int y = 0; y < boardCols; y++)
			boards[nowBoard].matrix[x][y].visibility = visibility_t::none;
}

/**
 * roll dice algorithm old
 * count = random initl..initr
 * do count times dice = random l..r
**/
int dice_old(int initl, int initr, int l, int r)
{
	int count = std::uniform_int_distribution<>(initl, initr)(gen), ans;
	for (int i = 0; i < count; i++)
		ans = std::uniform_int_distribution<>(l, r)(gen);
	return ans;
}

/**
 * simplified dice
**/
int dice(int l, int r)
{
	return std::uniform_int_distribution<>(l, r)(gen);
}

/**
 * draw current board(14x80)
**/
void drawBoard()
{
	for (int x = boardRows - 1; x >= 0; x--)
	{
		for (int y = 0; y < boardCols; y++)
		{
			int id = boards[nowBoard].matrix[x][y].terrainID;
			std::string terrain = terrains[id].name;
			if (!terrains[id].visible)
				terrain = "";
			if (x == nowRow && y == nowCol)
				terrain = "***";
			std::cout << terrain << std::string(width / boardCols - terrain.length(), ' ');
		}
		std::cout << std::endl;
		for (int y = 0; y < boardCols; y++)
		{
			std::string name = "";
			cell now = boards[nowBoard].matrix[x][y];
			if (now.visibility == visibility_t::exists)
				name = "Unknown";
			else if (now.visibility == visibility_t::visible)
			{
				name = stuffArray[now.type][now.stuffID].name;
				if (now.starID)
					name = "Vital";
			}
			std::cout << name << std::string(width / boardCols - name.length(), ' ');
		}
		std::cout << std::endl
				  << std::endl;
	}
}

/**
 * generate steps dice(1..catSpeed, 1..catSpeed)
 * modify msg
**/
void generateStep()
{
	remainStep = dice(1, catSpeed) + speedBonus;
	msg += "You can move up to " + std::to_string(remainStep) + " steps\n";
}

/**
 * process command
 * @cmd command to process
 * quit
 * walking:
 * u = up (walk up)
 * d = down
 * l = left
 * r = right
 * ul, ur, dl, dr ShadowClan only
 * s = stop (stop walking)
 * stopped:
 * b = battle [arg u|d|l|r|ul|ur|dl|dr]
 * h = hunt [arg]
 * drink
 * c = check
 * e = explore [arg]
 * r = rest
 * battling:
 * def = defend
 * f = flee
**/
void process(const std::string &cmd)
{
	if (cmd == "q" || cmd == "quit")
	{
		std::cout << "farewell\n";
		std::string tmp;
		getline(std::cin, tmp);
		exit(0);
	}
	if (remainStep)
	{
		//walking state
		if (cmd == "s" || cmd == "stop")
		{
			msg += "Stopped\n";
			remainStep = 0;
			loseVal();
		}
		else if (cmd == "u" || cmd == "up" || cmd == "d" || cmd == "down" || cmd == "l" || cmd == "left" || cmd == "r" || cmd == "right" || cmd == "ul" || cmd == "upleft" || cmd == "ur" || cmd == "upright" || cmd == "dl" || cmd == "downleft" || cmd == "dr" || cmd == "downright")
		{
			if (!findOpponent(cmd, true))
			{
				msg += "You can move up to " + std::to_string(remainStep) + " steps\n";
				return;
			}
			if (opponentCol < 0 || opponentCol == boardCols)
			{
				msg += "No way! You can move up to " + std::to_string(remainStep) + " steps\n";
				return;
			}
			if (opponentRow == -1)
			{
				if (--nowBoard == -1)
				{
					msg += "No way! You can move up to " + std::to_string(remainStep) + " steps\n";
					nowBoard++;
					return;
				}
				opponentRow = boardRows - 1;
				clearBoard();
			}
			if (opponentRow == boardRows)
			{
				if (++nowBoard == boardCount - 1)
				{
					msg += "You found Moonpool! ";
					if (starFound >= starCount)
					{
						std::cerr << msg << "You win with " << std::to_string(starFound) << " vital items found and earn your warrior name: " << prefixes[catName] << suffixes[catName] << std::endl;
						std::string tmp;
						getline(std::cin, tmp);
						exit(0);
					}
					msg += "But you only found " + std::to_string(starFound) + " vital items out of " + std::to_string(starCount) + ".\n";
				}
				opponentRow = 0;
				clearBoard();
			}
			walk();
		}
		else if (cmd == "help" || cmd == "?")
		{
			msg += "Commands: up, down, left, right, stop";
			if (diagonal)
				msg += ", ul(upleft), ur(upright), dl(downleft), dr(downright)\n";
			else
				msg += '\n';
			return;
		}
		else
		{
			msg += "Invalid command: unknown command or command that cannot be executed walking, see help\n";
			return;
		}
	}
	else if (battling)
	{
		enemy ene = enemies[boards[nowBoard].matrix[opponentRow][opponentCol].stuffID];
		if (cmd == "d" || cmd == "defend")
			battle();
		else if (cmd == "f" || cmd == "flee")
			flee();
		else if (cmd == "help" || cmd == "?")
		{
			msg += "Commands: defend, flee\n" + ene.name + "(" + std::to_string(enemyHealth) + ") attacks you, attack:" + std::to_string(enemyAttack) + '\n';
			return;
		}
		else
		{
			msg += "Invalid command: unknown command or command that cannot be executed battling, see help\n" + ene.name + "(" + std::to_string(enemyHealth) + ") attacks you, attack:" + std::to_string(enemyAttack) + '\n';
			return;
		}
	}
	else
	{
		if (cmd == "help" || cmd == "?")
		{
			msg += "Commands: battle, hunt, drink, check, explore, rest\n";
			msg += "Battle: Specify a cell to battle\n";
			msg += "Hunt: Hunt prey on a cell, walk directly into it will scare prey away\n";
			msg += "Drink: Drink water on current cell\n";
			msg += "Check: Show most of occupied cells\n";
			msg += "Explore: Try to figure out what occupies a nearby cell\n";
			msg += "Rest: Likely to gain some Health\n";
			return;
		}
		else if (cmd[0] == 'b' || cmd.substr(0, 6) == "battle")
		{
			std::size_t pos = cmd.find(' ');
			if (pos == std::string::npos)
			{
				msg += "You have to specify an opponent!\n";
				return;
			}
			if (!findOpponent(cmd.substr(pos + 1), false))
				return;
			if (boards[nowBoard].matrix[opponentRow][opponentCol].type == cell_t::Enemy)
			{
				battling = true;
				std::pair<int, int> range = enemies[boards[nowBoard].matrix[opponentRow][opponentCol].stuffID].health;
				enemyHealth = std::uniform_int_distribution<>(range.first, range.second)(gen);
				enemyAttack = 0;
				battle();
			}
			else
			{
				msg += "Nothing to battle\n";
				generateStep();
			}
		}
		else if (cmd[0] == 'h' || cmd.substr(0, 4) == "hunt")
		{
			std::size_t pos = cmd.find(' ');
			if (pos == std::string::npos)
			{
				msg += "You have to specify an opponent!\n";
				return;
			}
			if (!findOpponent(cmd.substr(pos + 1), false))
				return;
			if (boards[nowBoard].matrix[opponentRow][opponentCol].type == cell_t::Prey)
				hunt();
			else
			{
				msg += "Nothing to hunt\n";
				generateStep();
			}
		}
		else if (cmd == "d" || cmd == "drink")
		{
			if ((boards[nowBoard].matrix[nowRow][nowCol].terrainID >= 15 && boards[nowBoard].matrix[nowRow][nowCol].terrainID <= 18) || boards[nowBoard].matrix[nowRow][nowCol].terrainID == 28)
				drink();
			else
			{
				msg += "No water here\n";
				generateStep();
			}
		}
		else if (cmd == "c" || cmd == "check")
			check();
		else if (cmd[0] == 'e' || cmd.substr(0, 7) == "explore")
		{
			std::size_t pos = cmd.find(' ');
			if (pos == std::string::npos)
			{
				msg += "You have to specify an opponent!\n";
				return;
			}
			if (!findOpponent(cmd.substr(pos + 1), false))
				return;
			if (boards[nowBoard].matrix[opponentRow][opponentCol].type != cell_t::None)
				explore();
			else
			{
				msg += "Nothing here\n";
				generateStep();
			}
		}
		else if (cmd == "r" || cmd == "rest")
			rest();
		else
		{
			msg += "Invalid command: unknown command or command that cannot be executed stopped, see help\n";
			return;
		}
	}
}

/**
 * convert vector prey, injury, etc. to vector stuff
**/
template <class T>
std::vector<stuff> to_stuffs(const std::vector<T> &stuffs)
{
	std::vector<stuff> ans(stuffs.size());
	for (int i = 0; i < stuffs.size(); i++)
		ans[i] = stuffs[i];
	return ans;
}

/**
 * check cat state
 * if any value <= 0 then knocked out
**/
void checkState()
{
	if (inv)
		return;
	if (catHealth <= 0 || catHunger <= 0 || catThirst <= 0)
	{
		std::cout << "< " << msg;
		std::cerr << "You died! Meooooow!\n";
		std::string tmp;
		getline(std::cin, tmp);
		exit(0);
	}
}

/**
 * when stopped, lose some value
**/
void loseVal()
{
	if (++healthNow == healthMax)
	{
		catHealth--;
		healthNow = 0;
	}
	if (++hungerNow == hungerMax)
	{
		catHunger--;
		hungerNow = 0;
	}
	if (++thirstNow == thirstMax)
	{
		catThirst--;
		thirstNow = 0;
	}
	checkState();
}

/**
 * cat's turn
**/
void battle()
{
	cell &now = boards[nowBoard].matrix[opponentRow][opponentCol];
	enemy ene = enemies[now.stuffID];
	int def = dice(1, catDefense);
	int catLost = std::max(enemyAttack - def, 0);
	catHealth -= catLost;
	checkState();
	int atk = dice(1, catStrength + strengthBonus);
	def = dice(0, ene.defense);
	int enemyLost = std::max(atk - def, 0);
	enemyHealth -= enemyLost;
	msg += "You lost " + std::to_string(catLost) + " health, enemy lost " + std::to_string(enemyLost) + " health\n";
	if (enemyHealth <= 0)
	{
		if (now.starID)
		{
			msg += "You found " + stars[now.starID].name;
			++starFound;
		}
		else
			msg += "You defeated " + ene.name;
		msg += ", great work\n";
		now.type = cell_t::None;
		now.visibility = visibility_t::none;
		battling = false;
		nowRow = opponentRow;
		nowCol = opponentCol;
		generateStep();
	}
	else
		enemyTurn();
}

/**
 * return to a random col
**/
void flee()
{
	catHealth--;
	catHunger--;
	catThirst--;
	checkState();
	boards[nowBoard].matrix[opponentRow][opponentCol].visibility = visibility_t::none;
	battling = false;
	nowRow = 0;
	nowCol = dice(0, boardCols - 1);
	msg += "Running away will cost you 1 point each Health, Hunger, Thirst\n";
	generateStep();
}

/**
 * process anything on the way
**/
void walk()
{
	if (!--remainStep)
	{
		loseVal();
		msg += "Stopped\n";
	}
	nowRow = opponentRow;
	nowCol = opponentCol;
	cell &now = boards[nowBoard].matrix[nowRow][nowCol];
	if (now.terrainID > 14 && now.terrainID < 19 && waterp)
	{
		catHealth--;
		checkState();
	}
	if (now.type != cell_t::None)
	{
		now.visibility = visibility_t::visible;
		remainStep = 0;
		loseVal();
		if (now.type == cell_t::Prey)
		{
			msg += "You scared away some prey\n";
			now.type = cell_t::None;
			now.visibility = visibility_t::none;
			generateStep();
		}
		else if (now.type == cell_t::Enemy)
		{
			battling = true;
			std::pair<int, int> range = enemies[boards[nowBoard].matrix[opponentRow][opponentCol].stuffID].health;
			enemyHealth = dice(range.first, range.second);
			enemyTurn();
			battle();
		}
		else
		{
			processStuff();
			generateStep();
		}
	}
	else if (remainStep)
		msg += "You can move up to " + std::to_string(remainStep) + " steps\n";
}

/**
 * set opponent by s
**/
bool findOpponent(const std::string &s, bool bound)
{
	opponentRow = nowRow;
	opponentCol = nowCol;
	if (s == "u" || s == "up")
		opponentRow++;
	else if (s == "d" || s == "down")
		opponentRow--;
	else if (s == "l" || s == "left")
		opponentCol--;
	else if (s == "r" || s == "right")
		opponentCol++;
	else if (!diagonal && (s == "ul" || s == "upleft" || s == "ur" || s == "upright" || s == "dl" || s == "downleft" || s == "dr" || s == "downright"))
	{
		msg += "Invalid direction: only ShadowClan can walk diagonally\n";
		return false;
	}
	else if (s == "ul" || s == "upleft")
		opponentRow++, opponentCol--;
	else if (s == "ur" || s == "upright")
		opponentRow++, opponentCol++;
	else if (s == "dl" || s == "downleft")
		opponentRow--, opponentCol--;
	else if (s == "dr" || s == "downright")
		opponentRow--, opponentCol++;
	else
	{
		msg += "Invalid direction: unknown direction, see help\n";
		return false;
	}
	if (bound || (opponentRow >= 0 && opponentRow < boardRows && opponentCol >= 0 && opponentCol < boardCols))
		return true;
	msg += "Invalid direction: out of map!\n";
	return false;
}

void hunt()
{
	cell &opponent = boards[nowBoard].matrix[opponentRow][opponentCol];
	int result = dice(1, catSkill);
	prey opr = preys[opponent.stuffID];
	msg += "You rolled " + std::to_string(result) + " and requires >= " + std::to_string(opr.skill) + ".\n";
	if (result >= opr.skill)
	{
		msg += "You caught " + opr.name + " and received " + std::to_string(opr.hunger) + " hunger points! ";
		catHunger += opr.hunger;
		if (catHunger > catValMax)
			catHunger = catValMax;
	}
	else
		msg += "You lost " + opr.name + "! ";
	opponent.type = cell_t::None;
	opponent.visibility = visibility_t::none;
	nowRow = opponentRow;
	nowCol = opponentCol;
	generateStep();
}

void drink()
{
	int result = dice(1, 2);
	msg += "Good water, you gained " + std::to_string(result) + " thirst points!\n";
	catThirst += result;
	if (catThirst > catValMax)
		catThirst = catValMax;
	generateStep();
}

/**
 * check scent
**/
void check()
{
	for (int x = 0; x < boardRows; x++)
		for (int y = 0; y < boardCols; y++)
		{
			cell now = boards[nowBoard].matrix[x][y];
			if (now.type != cell_t::None && stuffArray[now.type][now.stuffID].scent)
				boards[nowBoard].matrix[x][y].visibility = visibility_t::exists;
			//overwrite visible
		}
	generateStep();
}

void explore()
{
	cell &opponent = boards[nowBoard].matrix[opponentRow][opponentCol];
	if (opponent.type != cell_t::None && stuffArray[opponent.type][opponent.stuffID].explore)
		opponent.visibility = visibility_t::visible;
	generateStep();
}

void rest()
{
	int result = std::uniform_int_distribution<>(1, 4)(gen);
	if (result == 1)
	{
		catHealth += 2;
		msg += "You received 2 health points!\n";
	}
	else if (result == 2 || result == 3)
	{
		catHealth++;
		msg += "You received 1 health points!\n";
	}
	else
		msg += "You did not receive any health points, maybe next time.\n";
	if (catHealth > catValMax)
		catHealth = catValMax;
	generateStep();
}

void processStuff()
{
	cell &opponent = boards[nowBoard].matrix[nowRow][nowCol];
	if (opponent.type == cell_t::Benefit)
	{
		benefit opb = benefits[opponent.stuffID];
		int result = dice(0, 6) + opb.dice;
		msg += "You rolled " + std::to_string(result) + " and requires > " + std::to_string(opb.requires) + ".\n";
		if (result > opb.requires)
		{
			msg += "You found " + opb.name + " and received " + std::to_string(opb.effect);
			if (opb.applyto == catVal_t::health)
			{
				msg += " health points! ";
				catHealth += opb.effect;
				if (catHealth > catValMax)
					catHealth = catValMax;
			}
			else if (opb.applyto == catVal_t::hunger)
			{
				msg += " hunger points! ";
				catHunger += opb.effect;
				if (catHunger > catValMax)
					catHunger = catValMax;
			}
			else
			{
				msg += " thirst points! ";
				catThirst += opb.effect;
				if (catThirst > catValMax)
					catThirst = catValMax;
			}
		}
		else
			msg += "You lost " + opb.name + "! ";
	}
	else if (opponent.type == cell_t::DefenseAction)
	{
		defenseAction opd = defenseActions[opponent.stuffID];
		int damage = dice(opd.damage.first, opd.damage.second);
		int result = dice(1, opd.apply == catVal_t::skill ? catSkill : catSpeed);
		msg += "You rolled " + std::to_string(result) + " and max damage is " + std::to_string(damage) + ".\n";
		result = std::max(damage - result, 0);
		msg += "You encountered " + opd.name + " and lost " + std::to_string(result) + " health points! ";
		catHealth -= result;
		checkState();
	}
	else
	{
		injury opi = injuries[opponent.stuffID];
		int result = dice(0, opi.damage);
		msg += "You rolled " + std::to_string(result) + " and max damage is " + std::to_string(opi.damage) + ".\n";
		msg += "You encountered " + opi.name + " and lost " + std::to_string(result) + " health points! ";
		catHealth -= result;
		checkState();
	}
	opponent.type = cell_t::None;
	opponent.visibility = visibility_t::none;
}

void enemyTurn()
{
	enemy ene = enemies[boards[nowBoard].matrix[opponentRow][opponentCol].stuffID];
	enemyAttack = dice(0, ene.strength);
	msg += ene.name + " (" + std::to_string(enemyHealth) + ") attacks you, attack: " + std::to_string(enemyAttack) + '\n';
}

void processParams(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		std::string s(argv[i]);
		if (s == "-i" || s == "--invincible")
			inv = true;
		else if (s == "--health")
		{
			catHealth = std::atoi(argv[++i]);
			catValMax = std::max(catValMax, catHealth);
		}
		else if (s == "--hunger")
		{
			catHunger = std::atoi(argv[++i]);
			catValMax = std::max(catValMax, catHealth);
		}
		else if (s == "--thirst")
		{
			catThirst = std::atoi(argv[++i]);
			catValMax = std::max(catValMax, catThirst);
		}
		else if (s == "--stepmax")
			healthMax = hungerMax = thirstMax = std::atoi(argv[++i]);
		else if (s == "--strength")
			catStrength = std::atoi(argv[++i]);
		else if (s == "--speed")
			catSpeed = std::atoi(argv[++i]);
		else if (s == "--skill")
			catSkill = std::atoi(argv[++i]);
		else if (s == "--defense")
			catDefense = std::atoi(argv[++i]);
		else if (s == "--no-water-penalty")
			waterp = false;
		else if (s == "--diagonal")
			diagonal = true;
		else if (s == "--cat-val-max")
			catValMax = std::atoi(argv[++i]);
	}
}