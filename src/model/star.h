#ifndef __STAR_H__
#define __STAR_H__
#include "../utils/multistring.h"
#include <vector>

/**
 * @brief The Star class
 * This class represents a vital item in the game.
 */
class Star
{
private:
	MultiString name;		  /* multi-language name */
	std::vector<int> enemies; /* not found yet, maybe not implemented in flash version? */

public:
	Star(MultiString name, std::vector<int> enemies);
	MultiString getName() const;
};

#endif