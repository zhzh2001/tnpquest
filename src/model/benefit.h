#ifndef __BENEFIT_H__
#define __BENEFIT_H__
#include "item.h"
#include "../model/cat.h"
#include "../utils/multistring.h"
#include <vector>

/**
 * @brief The Benefit class
 * represents a *type* of benefit item to be placed in the game board
 */
class Benefit : public Item
{
private:
	int dice;		/* dice(1..dice), > require to apply benefit */
	int require;	/* dice required to apply */
	int effect;		/* benefit points */
	CatVal applyto; /* apply to which cat value */

public:
	Benefit(MultiString name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, int dice, int require, int effect, CatVal applyto);

	/**
	 * @brief see if the cat can apply this benefit item, display the result
	 * @param cat the cat who encountered this item
	 */
	void trigger(Cat &cat) const override;
};

#endif