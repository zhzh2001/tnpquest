#ifndef __MAIN_H
#define __MAIN_H
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <algorithm>

const std::string version = "v0.2 alpha";
std::minstd_rand gen(time(NULL)); //global random engine

/***
 * attributes
 ***/
enum clan
{
	undefined,
	thunder, // +2 strength
	wind,	// +2 speed
	river,   // no water penalty
	shadow   // move diagonally
} catClan;
int catHealth;
int catHunger;
int catThirst;
int catStrength;
int catDefense;
int catSpeed;
int catSkill;
int nowBoard;
int nowRow;
int nowCol;
int healthNow; //once reach healthMax steps, catHealth--
int hungerNow; //same as healthNow
int thirstNow;
std::string msg;
int remainStep;
int strengthBonus;
int speedBonus;
bool battling;
int opponentRow;
int opponentCol;
int enemyAttack;
int enemyHealth;
int starFound;
bool inv = false;
bool waterp = true;
bool diagonal = false;
int catName;

/***
 * presets
 * some are changable
 ***/
std::uniform_int_distribution<> catVal(8, 12); //initial catHealth, catHunger & catThirst range
const int boardRows = 5;
const int boardCols = 8;
const int boardCount = 13; //boards in a game
const int width = 80;	  //target screen width
const int boardTotal = 40; //all boards, for generation
const int terrainCount = 34;
const int preyCount = 16;
const int injuryCount = 13;
const int benefitCount = 15;
const int defenseActionCount = 20;
const int enemyCount = 23;
const int starCount = 4;
int healthMax = 5; //go healthMax steps to decrease catHealth by 1
int hungerMax = 5; //same as healthMax
int thirstMax = 5;
int catValMax = 16;
const int names = 26;

/***
 * data structures
 ***/

/***
 * specific type of cell
 * star not included
 ***/
enum cell_t
{
	None,
	Prey,
	Injury,
	Benefit,
	DefenseAction,
	Enemy
};
/***
 * base class
 ***/
struct stuff
{
	std::string name;
	std::vector<int> counts; //random one for count of stuff
	std::vector<int> places; //random one for **terrain** of stuff
	bool explore;			 //is explorable
	bool scent;				 //is scentable
};
struct prey : stuff
{
	prey(std::string name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, int skill, int hunger) : stuff({name, counts, places, explore, scent}), skill(skill), hunger(hunger) {}
	int skill;  //dice(1..catSpeed, 1..catSkill), if result >= skill then prey caught
	int hunger; //bonus hunger
};
struct injury : stuff
{
	injury(std::string name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, int damage) : stuff({name, counts, places, explore, scent}), damage(damage) {}
	int damage; //decrease catHealth by dice(0..damage, 1..damage)
};
enum catVal_t
{
	health,
	hunger,
	thirst,
	strength,
	defense,
	speed,
	skill
};
struct benefit : stuff
{
	benefit(std::string name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, int dice, int requires, int effect, catVal_t applyto) : stuff({name, counts, places, explore, scent}), dice(dice), requires(requires), effect(effect), applyto(applyto) {}
	int dice; //dice(1..catSpeed, 1..dice), if result > requires then benefit got
	int requires;
	int effect; //benefit points
	catVal_t applyto;
};
struct defenseAction : stuff
{
	defenseAction(std::string name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, std::pair<int, int> damage, catVal_t apply) : stuff({name, counts, places, explore, scent}), damage(damage), apply(apply) {}
	std::pair<int, int> damage; //random real damage, dice(catSpeed, cat<apply>), decrease catHealth by real damage - result
	catVal_t apply;
};
struct enemy : stuff
{
	enemy(std::string name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, int strength, int defense, std::pair<int, int> health) : stuff({name, counts, places, explore, scent}), strength(strength), defense(defense), health(health) {}
	int strength;
	int defense;
	std::pair<int, int> health;
};
/***
 * battle
 * cat attack: dice(1..catSpeed, 1..catStrength)
 * cat defend: dice(1..catSpeed, 1..catDefense)
 * enemy attack: dice(1..15, 1..enemyStrength)
 * enemy defend: dice(1..15, 1..enemyDefense)
 ***/
struct star
{
	std::string name;
	std::vector<int> enemys; //not found yet, maybe not implemented in flash version?
};
struct terrain
{
	std::string name;
	bool visible; //useless, always true
};
struct boardTerrain
{
	int types[boardRows * boardCols];
};
enum visibility_t
{
	none,
	exists, //check scent result, a.k.a. red cells in flash version
	visible
};
struct cell
{
	cell_t type;
	int stuffID, terrainID, starID;
	visibility_t visibility;
};
struct board
{
	int id; //board id in all boards
	cell matrix[boardRows][boardCols];
} boards[boardCount];
struct cellPosition
{
	int boardID, row, col;
} opponent;
std::vector<cellPosition> terrainCells[terrainCount];
enum direction
{
	up,
	down,
	left,
	right,
	upleft,
	upright,
	downleft,
	downright
};

/***
 * functions
 * detailed instructions in main.cpp
 ***/
void printver();
void pregame();
void generateBoard();
void clearScreen();
void placeStuff(const std::vector<stuff> &stuffs, cell_t type);
void placeStar();
void doSPJ();
void clearBoard();
int dice(int l, int r);
void drawBoard();
void generateStep();
void process(const std::string &cmd);
template <class T>
std::vector<stuff> to_stuffs(const std::vector<T> &stuffs);
void walk();
void checkState();
void loseVal();
void battle();
void flee();
void hunt();
void drink();
void check();
void explore();
void rest();
bool findOpponent(const std::string &s, bool bound);
void processStuff();
void enemyTurn();
void processParams(int argc, char *argv[]);

#endif
