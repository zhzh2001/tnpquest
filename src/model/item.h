#ifndef __ITEM_H__
#define __ITEM_H__
#include "../utils/multistring.h"
#include "cat.h"
#include <vector>

/**
 * @brief The Item abstract class
 * represents a *type* of item to be placed in the game board
 */
class Item
{
private:
	MultiString name;
	std::vector<int> counts; /* random one for count of item */
	std::vector<int> places; /* random one for **terrain** of item */
	bool explore;			 /* is explorable */
	bool scent;				 /* is scentable */

public:
	Item(MultiString name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent);

	/* trivial getters */
	MultiString getName() const;
	const std::vector<int> &getCounts() const;
	const std::vector<int> &getPlaces() const;
	bool getExplore() const;
	bool getScent() const;

	/**
	 * @brief let the cat trigger this item, a pure virtual function to be implemented by subclasses
	 * @param cat the cat who triggered this item
	 */
	virtual void trigger(Cat &cat) const = 0;
};

#endif