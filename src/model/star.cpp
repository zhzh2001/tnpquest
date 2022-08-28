#include "star.h"

Star::Star(MultiString name, std::vector<int> enemies) : name(name), enemies(enemies)
{
}

MultiString Star::getName() const
{
	return name;
}