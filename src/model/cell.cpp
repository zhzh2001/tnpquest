#include "cell.h"
#include "enemy.h"
#include "star.h"
#include "../utils/utils.h"
#include "../utils/msgbox.h"
#ifdef _WIN32
#include "../pdcurses/curses.h"
#else
#include "../ncurses/ncurses.h"
#endif
#include <typeinfo>

Cell::Cell() : visibility(none), item(nullptr), terrain(nullptr), star(nullptr) {}

void Cell::setItem(const Item *item)
{
	this->item = item;
}

void Cell::setTerrain(const Terrain *terrain)
{
	this->terrain = terrain;
}

void Cell::setStar(const Star *star)
{
	this->star = star;
}

void Cell::setVisibility(Visibility visibility)
{
	if (!item)
		return;
	if (visibility == none)
		this->visibility = none;
	else if (visibility == exists) // try to set it to exists
	{
		if (item->getScent())
			this->visibility = exists; // item is scentable
		else
			this->visibility = none; // item is not scentable, so it's not visible
	}
	else if (visibility == visible) // try to set it to visible
	{
		if (item->getExplore()) // explorable implys scentable
			this->visibility = visible;
		else if (item->getScent())
			this->visibility = exists;
		else
			this->visibility = none;
	}
}

void Cell::renderTerrain(bool current)
{
	if (terrain)
		printw(paddingMid(std::string(terrain->getName()), current).c_str());
	else
		printw("ERROR!!!"); // unexpected error
}

void Cell::renderItem(bool current)
{
	if (!item)
		printw("        ");
	else if (visibility == visible)
	{
		if (star) // print star first
			printw(MultiString({" Vital  ", " 必需品 "}));
		else
			printw(paddingMid(std::string(item->getName()), current).c_str());
	}
	else if (visibility == exists)
		printw(paddingMid(std::string(MultiString({"Unknown", "未知"})), current).c_str());
	else
		printw("        ");
}

bool Cell::trigger(Cat &cat)
{
	if (!item)
		return false;
	item->trigger(cat);
	if (typeid(*item) == typeid(Enemy)) // use RTTI to check if the item is an enemy
	{
		if (cat.hasFled())
			return true;
		std::string msg;
		if (star)
		{
			msg = MultiString({"You found " + star->getName(), "你找到了" + star->getName()});
			cat.incStar();
		}
		else
			msg = MultiString({"You defeated " + item->getName(), "你击败了" + item->getName()});
		msg += MultiString({", great work!", ", 太棒了!"});
		MsgBox::create(msg.c_str());
	}
	item = nullptr; // remove triggered item
	return true;
}

bool Cell::isEnemy() const
{
	return item && typeid(*item) == typeid(Enemy);
}

bool Cell::hunt(Cat &cat)
{
	if (item && typeid(*item) == typeid(Prey))
	{
		dynamic_cast<const Prey *>(item)->hunt(cat); // don't use trigger() here, as it will cause the prey to be scared away
		return true;
	}
	MsgBox::create(MultiString({"Nothing to hunt here.", "这里没有可以捕猎的东西。"}));
	return false;
}

const Terrain *Cell::getTerrain() const
{
	return terrain;
}