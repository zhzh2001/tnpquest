#include "item.h"

Item::Item(MultiString name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent) : name(name), counts(counts), places(places), explore(explore), scent(scent) {}

MultiString Item::getName() const
{
	return name;
}

const std::vector<int> &Item::getCounts() const
{
	return counts;
}

const std::vector<int> &Item::getPlaces() const
{
	return places;
}

bool Item::getExplore() const
{
	return explore;
}

bool Item::getScent() const
{
	return scent;
}