#include "enemy.h"
#include "../controller/game.h"
#include "../utils/msgbox.h"

Enemy::Enemy(MultiString name, std::vector<int> counts, std::vector<int> places, bool explore, bool scent, int strength, int defense, std::pair<int, int> healthRange) : Item(name, counts, places, explore, scent), strength(strength), defense(defense), healthRange(healthRange) {}

void Enemy::trigger(Cat &cat) const
{
	int enemyHealth = std::uniform_int_distribution<>(healthRange.first, healthRange.second)(Game::getRng());
	int enemyAttack = std::uniform_int_distribution<>(0, strength)(Game::getRng());
	bool first = true; // first round, not allowed to flee
	do
	{
		std::string msg(MultiString({getName() + "(" + std::to_string(enemyHealth) + ") attacked you, attack: " + std::to_string(enemyAttack),
									 getName() + "(" + std::to_string(enemyHealth) + ") 攻击了你, 攻击力: " + std::to_string(enemyAttack)}));
		if (first)
			MsgBox::create(msg.c_str());
		else
		{
			int option = MsgBox::create(msg.c_str(), MultiString({"Defend", "防御"}), MultiString({"Flee", "逃跑"}));
			if (option == 2)
			{
				cat.flee();
				return;
			}
		}
		first = false;
		int catDefense = cat.rollDefense();
		int dmg = std::max(0, enemyAttack - catDefense); // damage is always positive
		cat.loseHealth(dmg);
		int catAttack = cat.rollAttack();
		int enemyDefense = std::uniform_int_distribution<>(0, defense)(Game::getRng());
		int dmg2 = std::max(0, catAttack - enemyDefense);
		enemyHealth -= dmg2;
		MsgBox::create(MultiString({"You lost " + std::to_string(dmg) + " health, enemy lost " + std::to_string(dmg2) + " health", "你受到了 " + std::to_string(dmg) + " 点伤害, 敌人受到了 " + std::to_string(dmg2) + " 点伤害"}));
	} while (enemyHealth > 0);
}