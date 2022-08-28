#include "injury.h"
#include "../controller/game.h"
#include "../utils/msgbox.h"

Injury::Injury(MultiString name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, int damage) : Item(name, counts, places, explore, scent), damage(damage) {}

void Injury::trigger(Cat &cat) const
{
	int damage = std::uniform_int_distribution<>(1, this->damage)(Game::getRng());
	MsgBox::create(MultiString({"You rolled " + std::to_string(damage) + " and max damage is " + std::to_string(damage),
								"你掷出了 " + std::to_string(damage) + " 点伤害，最大伤害为 " + std::to_string(damage)}),
				   MultiString({"You encontered " + getName() + " and lost " + std::to_string(damage) + " health.", "你遇到了" + getName() + "，失去了 " + std::to_string(damage) + " 点生命值。"}));
	cat.loseHealth(damage);
}