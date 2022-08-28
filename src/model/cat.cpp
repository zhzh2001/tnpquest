#include "cat.h"
#include "board.h"
#include "../controller/game.h"
#include "../utils/msgbox.h"
#ifdef _WIN32
#include "../pdcurses/curses.h"
#else
#include "../ncurses/ncurses.h"
#endif

const MultiString Cat::prefixes[26] = {
	MultiString({"Hawk", "鹰"}),
	MultiString({"Tangle", "纠缠"}),
	MultiString({"Wild", "野"}),
	MultiString({"Night", "夜"}), // wow, you discovered an easter egg!
	MultiString({"Rain", "雨"}),
	MultiString({"Thorn", "刺"}),
	MultiString({"Fuzzy", "绒"}),
	MultiString({"Mud", "泥"}),
	MultiString({"Loud", "响"}),
	MultiString({"Bramble", "黑莓"}),
	MultiString({"Moss", "苔"}),
	MultiString({"Leopard", "豹"}),
	MultiString({"Stealth", "隐"}),
	MultiString({"Bright", "亮"}),
	MultiString({"Running", "奔"}),
	MultiString({"Sand", "沙"}),
	MultiString({"Dawn", "曙"}),
	MultiString({"Ice", "冰"}),
	MultiString({"Spotted", "斑"}),
	MultiString({"Rock", "岩"}),
	MultiString({"Red", "红"}),
	MultiString({"Mouse", "鼠"}),
	MultiString({"Tiger", "虎"}),
	MultiString({"Flower", "花"}),
	MultiString({"Claw", "爪"}),
	MultiString({"Blue", "蓝"})};

const MultiString Cat::suffixes[26] = {
	MultiString({"fire", "火"}),
	MultiString({"claw", "掌"}),
	MultiString({"talon", "爪"}),
	MultiString({"eyes", "眼"}),
	MultiString({"fur", "毛"}),
	MultiString({"tail", "尾"}),
	MultiString({"nose", "鼻"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"}),
	MultiString({"fire", "火"})};

Cat::Cat() : clan(undefined),
			 health(0), hunger(0), thirst(0), maxVal(16), strength(0), defense(0), speed(0), skill(0), invincible(false), waterproof(false), diagonal(false), name(0), maxStep(5), curStep(0), fled(false), stars(0)
{
	MsgBox::initCat(this);
}

Cat::~Cat() {}

bool Cat::setClan(char c)
{
	switch (c)
	{
	case 't':
		clan = thunder;
		break;
	case 'w':
		clan = wind;
		break;
	case 'r':
		clan = river;
		waterproof = true;
		break;
	case 's':
		clan = shadow;
		diagonal = true;
		break;
	default:
		printw(MultiString({"\nInvalid clan\n", "\n无效的族群\n"}));
		return false;
	}
	return true;
}

void Cat::setHealth(int h)
{
	health = h;
	maxVal = std::max(maxVal, health);
}

void Cat::setHunger(int h)
{
	hunger = h;
	maxVal = std::max(maxVal, hunger);
}

void Cat::setThirst(int t)
{
	thirst = t;
	maxVal = std::max(maxVal, thirst);
}

void Cat::setStrength(int s)
{
	strength = s;
}

void Cat::setDefense(int d)
{
	defense = d;
}

void Cat::setSpeed(int s)
{
	speed = s;
}

void Cat::setSkill(int s)
{
	skill = s;
}

void Cat::setMaxVal(int m)
{
	maxVal = m;
}

void Cat::randomAttributes()
{
	std::minstd_rand &rng = Game::getRng();
	std::uniform_int_distribution<> catVal(maxVal / 2, maxVal * 3 / 4);
	if (!health)
		health = catVal(rng);
	if (!hunger)
		hunger = catVal(rng);
	if (!thirst)
		thirst = catVal(rng);
	if (!strength)
		strength = catVal(rng);
	if (!speed)
		speed = catVal(rng);
	if (!skill)
		skill = catVal(rng);
	if (!defense)
		defense = catVal(rng);
}

void Cat::setInvincible()
{
	invincible = true;
}

void Cat::setWaterproof()
{
	waterproof = true;
}

void Cat::setDiagonal()
{
	diagonal = true;
}

bool Cat::setName(char *s)
{
	name = std::tolower(s[0]) - 'a';
	if (name < 0 || name > 25)
	{
		printw(MultiString({"Invalid name\n", "无效的名字\n"}));
		return false;
	}
	return true;
}

std::string Cat::getApprenticeName() const
{
	return std::string(prefixes[name]) + std::string(MultiString({"paw", "爪"}));
}

std::string Cat::getWarriorName() const
{
	return std::string(prefixes[name]) + std::string(suffixes[name]);
}

void Cat::renderAttributes(bool fallback) const
{
	mvprintw(Board::rows * 3 + 1, 0, fallback ? "+" : "┌");
	for (int i = 1; i < 77; i++)
		if (fallback)
			printw(i % 11 ? "-" : "+");
		else
			printw(i % 11 ? "─" : "┬");
	printw(fallback ? "+" : "┐");
	if (fallback)
		mvprintw(Board::rows * 3 + 2, 0, MultiString({"|  Health  |  Hunger  |  Thirst  | Strength |  Speed   |  Skill   | Defense  |", "|   健康   |   饥饿   |   渴饮   |   力量   |   速度   |   技巧   |   防御   |"}));
	else
		mvprintw(Board::rows * 3 + 2, 0, MultiString({"│  Health  │  Hunger  │  Thirst  │ Strength │  Speed   │  Skill   │ Defense  │", "│   健康   │   饥饿   │   渴饮   │   力量   │   速度   │   技巧   │   防御   │"}));
	mvprintw(Board::rows * 3 + 3, 0, fallback ? "| %2d / %2d  | %2d / %2d  | %2d / %2d  |  %2d + %1d  |  %2d + %1d  |    %2d    |    %2d    |" : "│ %2d / %2d  │ %2d / %2d  │ %2d / %2d  │  %2d + %1d  │  %2d + %1d  │    %2d    │    %2d    │", health, maxVal, hunger, maxVal, thirst, maxVal, strength, clan == thunder ? 2 : 0, speed, clan == wind ? 2 : 0, skill, defense);
	mvprintw(Board::rows * 3 + 4, 0, fallback ? "+" : "├");
	for (int i = 1; i < 77; i++)
		if (fallback)
			printw(i % 11 ? "-" : "+");
		else
			printw(i % 11 ? "─" : (i <= 55 ? "┼" : "┴"));
	printw(fallback ? "+" : "┘");
}

Cat::Clan Cat::getClan() const
{
	return clan;
}

int Cat::generateStep() const
{
	std::uniform_int_distribution<> step(1, speed);
	return step(Game::getRng()) + (clan == wind ? 2 : 0); // WindClan speed bonus
}

void Cat::setMaxStep(int m)
{
	maxStep = m;
}

void Cat::loseVal()
{
	if (++curStep == maxStep)
	{
		curStep = 0;
		health--;
		hunger--;
		thirst--;
		checkDead();
	}
}

void Cat::checkDead() const
{
	if (invincible)
		return;
	if (health <= 0 || hunger <= 0 || thirst <= 0) // sadly you die
	{
		MsgBox::create(MultiString({"You died! Meooooow!", "你死了！喵——！"}));
		endwin(); // don't forget to end curses
		exit(0);
	}
}

void Cat::loseHealth(int h)
{
	health -= h;
	checkDead();
}

void Cat::gainHealth(int h)
{
	health += h;
	if (health > maxVal)
		health = maxVal;
}

void Cat::gainHunger(int h)
{
	hunger += h;
	if (hunger > maxVal)
		hunger = maxVal;
}

void Cat::gainThirst(int h)
{
	thirst += h;
	if (thirst > maxVal)
		thirst = maxVal;
}

int Cat::rollVal(CatVal val) const
{
	int target;
	if (val == CatVal::skill)
		target = skill;
	else
		target = speed;
	std::uniform_int_distribution<> roll(1, target);
	return roll(Game::getRng());
}

int Cat::rollAttack() const
{
	std::uniform_int_distribution<> roll(1, strength + (clan == thunder ? 2 : 0)); // ThunderClan strength bonus
	return roll(Game::getRng());
}

int Cat::rollDefense() const
{
	std::uniform_int_distribution<> roll(1, defense);
	return roll(Game::getRng());
}

void Cat::flee()
{
	health--;
	hunger--;
	thirst--;
	checkDead();
	fled = true;
	MsgBox::create(MultiString({"Running away will cost you 1 point each Health, Hunger, Thirst.", "逃跑将会消耗你1点健康、饥饿、渴饮。"}));
}

bool Cat::hasFled()
{
	return fled;
}

void Cat::resetFled()
{
	fled = false;
}

void Cat::incStar()
{
	stars++;
}

int Cat::getStars() const
{
	return stars;
}

bool Cat::getDiagonal() const
{
	return diagonal;
}

bool Cat::getWaterproof() const
{
	return waterproof;
}

void Cat::rest()
{
	int res = std::uniform_int_distribution<>(1, 4)(Game::getRng());
	if (res < 4)
	{
		int gain = res == 1 ? 1 : 2;
		gainHealth(gain);
		MsgBox::create(MultiString({"You received " + std::to_string(gain) + " health.", "你获得了 " + std::to_string(gain) + " 点健康。"}));
	}
	else
		MsgBox::create(MultiString({"You did not receive any health, maybe next time.", "你没有获得任何健康，可能下次再来。"}));
}

void Cat::drink()
{
	int res = std::uniform_int_distribution<>(1, 2)(Game::getRng());
	gainThirst(res);
	MsgBox::create(MultiString({"You received " + std::to_string(res) + " thirst.", "你获得了 " + std::to_string(res) + " 点渴饮。"}));
}