#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "item.h"
#include "terrain.h"
#include "../utils/multistring.h"
#include <vector>

/**
 * @brief The Enemy class
 * represents a *type* of enemy item to be placed in the game board
 */
class Enemy : public Item
{
private:
	int strength;					 /* strength of enemy (attack range) */
	int defense;					 /* defense of enemy */
	std::pair<int, int> healthRange; /* initial health range */

public:
	Enemy(MultiString name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, int strength, int defense, std::pair<int, int> healthRange);

	/**
	 * @brief the cat fights the enemy round-based, display the process and also allow the cat to flee
	 * @param cat the cat who encountered this enemy
	 */
	void trigger(Cat &cat) const override;
};

#endif