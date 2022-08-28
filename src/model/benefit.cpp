#include "benefit.h"
#include "../controller/game.h"
#include "../utils/msgbox.h"

Benefit::Benefit(MultiString name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, int dice, int require, int effect, CatVal applyto) : Item(name, counts, places, explore, scent), dice(dice), require(require), effect(effect), applyto(applyto) {}

void Benefit::trigger(Cat &cat) const
{
	int effect = std::uniform_int_distribution<>(0, 6)(Game::getRng()) + dice;
	std::string msg(MultiString({"You rolled " + std::to_string(effect) + " and requires > " + std::to_string(require) + " to apply the effect.", "你掷出了 " + std::to_string(effect) + "，需要 " + std::to_string(require) + " 以上才能使用效果。"}));
	std::string msg2;
	if (effect > require)
	{
		msg2 = MultiString({"You found " + getName() + " and received " + std::to_string(effect), "你找到了" + getName() + "，获得了 " + std::to_string(effect)});
		switch (applyto)
		{
		case CatVal::health:
			msg2 += MultiString({" health.", " 生命值。"});
			cat.gainHealth(effect);
			break;
		case CatVal::hunger:
			msg2 += MultiString({" hunger.", " 饥饿值。"});
			cat.gainHunger(effect);
			break;
		case CatVal::thirst:
			msg2 += MultiString({" thirst.", " 渴望值。"});
			cat.gainThirst(effect);
			break;
		default:
			msg = MultiString({"ERROR: Unknown cat value.", "错误：未知的猫值。"}); // unexpected error
			break;
		}
	}
	else
		msg2 += MultiString({"You lost " + getName() + "!", "你失去了" + getName() + "！"});
	MsgBox::create(msg.c_str(), msg2.c_str());
}