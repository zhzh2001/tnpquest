#include "prey.h"
#include "../utils/msgbox.h"
#include "../utils/multistring.h"

Prey::Prey(MultiString name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, int skill, int hunger) : Item(name, counts, places, explore, scent), skill(skill), hunger(hunger) {}

void Prey::trigger(Cat &cat) const
{
	MsgBox::create(MultiString({"You scared away some prey!", "你吓跑了一些猎物！"}));
}

void Prey::hunt(Cat &cat) const
{
	int res = cat.rollVal(CatVal::skill);
	std::string msg(MultiString({"You rolled " + std::to_string(res) + " and requires >= " + std::to_string(skill) + " to catch", "你掷出了 " + std::to_string(res) + "，需要 >= " + std::to_string(skill) + " 才能捕获"}));
	if (res >= skill)
	{
		MsgBox::create(msg.c_str(), MultiString({"You caught " + getName() + " and received " + std::to_string(hunger) + " hunger", "你捕获了" + getName() + "，获得了 " + std::to_string(hunger) + " 饥饿值"}));
		cat.gainHunger(hunger);
	}
	else
		MsgBox::create(msg.c_str(), MultiString({"You lost " + getName() + "!", "你没能捕获" + getName() + "！"}));
}