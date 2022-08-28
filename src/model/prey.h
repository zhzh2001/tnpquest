#ifndef __PREY_H__
#define __PREY_H__
#include "item.h"
#include "../utils/multistring.h"
#include <vector>

/**
 * @brief The Prey class
 * represents a *type* of prey item to be placed in the game board
 */
class Prey : public Item
{
private:
	int skill;	/* dice(1..catSkill), >= skill to catch */
	int hunger; /* bonus hunger */

public:
	Prey(MultiString name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, int skill, int hunger);

	/**
	 * @brief alert the cat scared some prey away
	 * @param cat the cat who encountered this prey
	 */
	void trigger(Cat &cat) const override;

	/**
	 * @brief see if the cat can catch this prey, display the result
	 * @param cat the cat who encountered this prey
	 */
	void hunt(Cat &cat) const;
};

#endif