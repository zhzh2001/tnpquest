#ifndef __DEFENSE_H__
#define __DEFENSE_H__
#include "item.h"
#include "../model/cat.h"
#include "../utils/multistring.h"
#include <vector>

/**
 * @brief The Defense action class
 * represents a *type* of defense item to be placed in the game board
 */
class DefenseAction : public Item
{
private:
	std::pair<int, int> damageRange; /* max damage range */
	CatVal applyfrom;				 /* roll dice according to which cat value */

public:
	DefenseAction(MultiString name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, std::pair<int, int> damageRange, CatVal applyfrom);

	/**
	 * @brief see if the cat can defend itself from danger, display the result damage
	 * @param cat the cat who encountered this item
	 */
	void trigger(Cat &cat) const override;
};

#endif