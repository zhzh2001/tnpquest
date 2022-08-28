#include "defense.h"
#include "../controller/game.h"
#include "../utils/msgbox.h"

DefenseAction::DefenseAction(MultiString name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, std::pair<int, int> damageRange, CatVal applyfrom) : Item(name, counts, places, explore, scent), damageRange(damageRange), applyfrom(applyfrom) {}

void DefenseAction::trigger(Cat &cat) const
{
	int damage = std::uniform_int_distribution<>(damageRange.first, damageRange.second)(Game::getRng());
	int result = cat.rollVal(applyfrom);
	std::string msg(MultiString({"You rolled " + std::to_string(result) + " and max damage is " + std::to_string(damage) + ".", "你掷出了" + std::to_string(result) + "，最大伤害是 " + std::to_string(damage) + "。"}));
	result = std::max(damage - result, 0); // damage is always positive
	std::string msg2(MultiString({"You encontered " + getName() + " and lost " + std::to_string(result) + " health", "你遇到了" + getName() + "，失去了 " + std::to_string(result) + " 点生命值。"}));
	MsgBox::create(msg.c_str(), msg2.c_str());
	cat.loseHealth(result);
}