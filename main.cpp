#include "main.h"
#include "data.h"

int main(int argc, char *argv[])
{
	printver();
	pregame();
	generateBoard();
	nowCol = std::uniform_int_distribution<>(0, boardCols - 1)(gen);
	generateStep();
	for (;;)
	{
		clearScreen();
		drawBoard();
		std::cout << "Health: " << catHealth << " Hunger: " << catHunger << " Thirst: " << catThirst << " Strength: " << catStrength << " + " << strengthBonus << " Defense: " << catDefense << " Speed: " << catSpeed << " + " << speedBonus << std::endl;
		std::cout << "Skill: " << catSkill << " Stars: " << starFound << " / " << starCount << " Status: " << (remainStep ? "Walking" : battling ? "Battling" : "Stopped") << std::endl;
		std::cout << "< " << msg << std::endl;
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
	std::cout << "Warrior Cats The New Prophecy Quest Game Mimicked CLI Version " << version << std::endl;
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
		std::cout << "Choose your Clan(t/w/r/s):" << std::flush;
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
			break;
		case 's':
			catClan = shadow;
			break;
		default:
			catClan = undefined;
			std::cout << "Unrecognizable Clan" << std::endl;
		}
	} while (catClan == undefined);
	std::string tmp;
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
			boards[nowBoard].matrix[nowRow][nowCol].visibility = visibility_t::none;
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
			if (now.visibility == exists)
				name = "Unknown";
			else if (now.visibility == visible)
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
	msg += "You can move up to " + std::to_string(remainStep) + " steps";
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
		exit(0);
	}
	if (remainStep)
	{
		//walking state
		if (cmd == "s")
		{
			msg = "Stopped";
			remainStep = 0;
			loseVal();
		}
		else if (cmd == "u" || cmd == "d" || cmd == "l" || cmd == "r" || cmd == "ul" || cmd == "ur" || cmd == "dl" || cmd == "dr")
		{
			findOpponent(cmd);
			if (opponentCol < 0 || opponentCol == boardCols)
				return;
			if (opponentRow == -1)
			{
				if (!nowBoard)
					return;
				nowBoard--;
				opponentRow = boardRows - 1;
				clearBoard();
			}
			if (opponentRow == boardRows)
			{
				if (nowBoard == boardCount)
					return;
				nowBoard++;
				opponentRow = 0;
				clearBoard();
			}
			walk();
			if (remainStep)
				msg = "You can move up to " + std::to_string(remainStep) + " steps";
		}
		else if (cmd == "help")
		{
			msg = "Commands: u(up), d(down), l(left), r(right), s(stop)";
			if (catClan == shadow)
				msg += ", ul(upleft), ur(upright), dl(downleft), dr(downright)";
			return;
		}
		else
		{
			msg = "Invalid command: unknown command or command that cannot be executed walking";
			return;
		}
	}
	else if (battling)
	{
		enemy ene = enemies[boards[nowBoard].matrix[opponentRow][opponentCol].stuffID];
		if (cmd == "defend")
			battle();
		else if (cmd == "f")
			flee();
		else if (cmd == "help")
		{
			msg = "Commands: defend, f(flee)\n" + ene.name + "(" + std::to_string(enemyHealth) + ") attacks you, attack:" + std::to_string(enemyAttack);
			return;
		}
		else
		{
			msg = "Invalid command: unknown command or command that cannot be executed battling\n" + ene.name + "(" + std::to_string(enemyHealth) + ") attacks you, attack:" + std::to_string(enemyAttack);
			return;
		}
	}
	else
	{
		if (cmd[0] == 'b')
		{
			if (findOpponent(cmd.substr(2)) && boards[nowBoard].matrix[opponentRow][opponentCol].type == cell_t::Enemy)
			{
				battling = true;
				std::pair<int, int> range = enemies[boards[nowBoard].matrix[opponentRow][opponentCol].stuffID].health;
				enemyHealth = std::uniform_int_distribution<>(range.first, range.second)(gen);
				enemyTurn();
			}
			else
			{
				msg = "Nothing to battle";
				return;
			}
		}
		else if (cmd.substr(0, 2) == "h ")
		{
			if (findOpponent(cmd.substr(2)) && boards[nowBoard].matrix[opponentRow][opponentCol].type == cell_t::Prey)
				hunt();
			else
			{
				msg = "Nothing to hunt";
				return;
			}
		}
		else if (cmd == "drink")
		{
			if ((boards[nowBoard].matrix[nowRow][nowCol].terrainID >= 15 && boards[nowBoard].matrix[nowRow][nowCol].terrainID <= 18) || boards[nowBoard].matrix[nowRow][nowCol].terrainID == 28)
				drink();
			else
			{
				msg = "No water here";
				return;
			}
		}
		else if (cmd[0] == 'c')
			check();
		else if (cmd[0] == 'e')
		{
			if (findOpponent(cmd.substr(2)) && boards[nowBoard].matrix[opponentRow][opponentCol].type != cell_t::None)
				explore();
			else
			{
				msg = "Nothing here";
				return;
			}
		}
		else if (cmd == "r")
			rest();
		else if (cmd == "help")
		{
			msg = "Commands: b(battle), h(hunt), drink, c(check), e(explore), r(rest)";
			return;
		}
		else
		{
			msg = "Invalid command: unknown command or command that cannot be executed stopped";
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
	if (catHealth <= 0 || catHunger <= 0 || catThirst <= 0)
	{
		std::cout << "< " << msg << std::endl;
		std::cerr << "You died! Meooooow!\n";
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
	if (enemyHealth <= 0)
	{
		if (now.starID)
		{
			msg = "You found " + stars[now.starID].name;
			++starFound;
		}
		else
			msg = "You defeated " + ene.name;
		msg += ", great work\n";
		now.type = cell_t::None;
		now.visibility = visibility_t::none;
		battling = false;
		generateStep();
	}
	else
	{
		msg = "You lost " + std::to_string(catLost) + " health, enemy lost " + std::to_string(enemyLost) + " health\n";
		enemyTurn();
	}
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
	msg = "Running away will cost you 1 point each Health, Hunger, Thirst\n";
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
		msg = "Stopped";
	}
	nowRow = opponentRow;
	nowCol = opponentCol;
	cell &now = boards[nowBoard].matrix[nowRow][nowCol];
	if (now.type != cell_t::None)
	{
		now.visibility = visibility_t::visible;
		remainStep = 0;
		loseVal();
		if (now.type == cell_t::Prey)
		{
			msg = "You scared away some prey";
			now.type = cell_t::None;
			now.visibility = visibility_t::none;
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
			processStuff();
	}
}

/**
 * set opponent by s
**/
bool findOpponent(const std::string &s)
{
	opponentRow = nowRow;
	opponentCol = nowCol;
	if (s == "u")
		opponentRow++;
	else if (s == "d")
		opponentRow--;
	else if (s == "l")
		opponentCol--;
	else if (s == "r")
		opponentCol++;
	if (catClan != shadow && (s == "ul" || s == "ur" || s == "dl" || s == "dr"))
	{
		msg = "Invalid command: only ShadowClan can walk diagonally";
		return false;
	}
	if (s == "ul")
		opponentRow++, opponentCol--;
	else if (s == "ur")
		opponentRow++, opponentCol++;
	else if (s == "dl")
		opponentRow--, opponentCol--;
	else if (s == "dr")
		opponentRow--, opponentCol++;
	return opponentRow >= 0 && opponentRow < boardRows && opponentCol >= 0 && opponentCol < boardCols;
}

void hunt()
{
	cell &opponent = boards[nowBoard].matrix[opponentRow][opponentCol];
	int result = dice(1, catSkill);
	prey opr = preys[opponent.stuffID];
	if (result >= opr.skill)
	{
		msg = "You caught " + opr.name + " and received " + opr.hunger + " hunger points!\n";
		catHunger += opr.hunger;
		if (catHunger > catValMax)
			catHunger = catValMax;
	}
	else
		msg = "You lost " + opr.name + "!\n";
	opponent.type = cell_t::None;
	opponent.visibility = visibility_t::none;
	nowRow = opponentRow;
	nowCol = opponentCol;
	generateStep();
}

void drink()
{
	int result = dice(1, 3);
	msg = "Good water, you gained " + result + " thirst points!\n";
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
		msg = "You received 2 health points!\n";
	}
	else if (result == 2 || result == 3)
	{
		catHealth++;
		msg = "You received 1 health points!\n";
	}
	else
		msg = "You did not receive any health points, maybe next time.\n";
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
		int result = dice(1, opb.dice);
		if (result > opb.requires)
		{
			msg = "You found " + opb.name + " and received " + opb.effect;
			if (opb.applyto == catVal_t::health)
			{
				msg += " health points!\n";
				catHealth += opb.effect;
			}
			else if (opb.applyto == catVal_t::hunger)
			{
				msg += " hunger points!\n";
				catHunger += opb.effect;
			}
			else
			{
				msg += " thirst points!\n";
				catThirst += opb.effect;
			}
		}
		else
			msg = "You lost " + opb.name + " !\n";
	}
	else if (opponent.type == cell_t::DefenseAction)
	{
		defenseAction opd = defenseActions[opponent.stuffID];
		int result = std::max(dice(opd.damage.first, opd.damage.second) - dice(1, opd.apply == catVal_t::skill ? catSkill : catSpeed), 0);
		msg += "You encountered " + opd.name + " and lost " + result + " health points!\n";
		catHealth -= result;
		checkState();
	}
	else
	{
		injury opi = injuries[opponent.stuffID];
		int result = dice(0, opi.damage);
		msg += "You encountered " + opi.name + " and lost " + result + " health points!\n";
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
	msg += ene.name + " (" + std::to_string(enemyHealth) + ") attacks you, attack: " + std::to_string(enemyAttack);
}