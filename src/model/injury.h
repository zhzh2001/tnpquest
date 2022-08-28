#ifndef __INJURY_H__
#define __INJURY_H__
#include "item.h"
#include <string>
#include <vector>

/**
 * @brief The Injury class
 * represents a *type* of injury item to be placed in the game board
 */
class Injury : public Item
{
private:
	int damage; /* max damage */

public:
	Injury(MultiString name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, int damage);

	/**
	 * @brief see how many damage the cat will take from this injury item, display the result
	 * @param cat the cat who encountered this item
	 */
	void trigger(Cat &cat) const override;
};

#endif