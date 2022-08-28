#ifndef __CAT_H__
#define __CAT_H__
#include "../utils/multistring.h"
#include <random>

/* cat value type */
enum CatVal
{
	health,
	hunger,
	thirst,
	strength,
	defense,
	speed,
	skill
};

/**
 * @brief The Cat class
 * control cat attributes and actions
 */
class Cat
{
public:
	/* cat clan */
	enum Clan
	{
		undefined,
		thunder,
		wind,
		river,
		shadow
	};

private:
	Clan clan;
	int health;
	int hunger;
	int thirst;
	int maxVal; /* max value for health, hunger, thirst */
	int strength;
	int defense;
	int speed;
	int skill;
	bool invincible;					   /* if true, cat won't die */
	bool waterproof;					   /* if true, cat won't lose health in water */
	bool diagonal;						   /* if true, cat can move diagonally */
	int name;							   /* name index in name list */
	int maxStep;						   /* how many rounds cat can go before losing value */
	int curStep;						   /* current rounds */
	bool fled;							   /* if true, cat fled from battle */
	int stars;							   /* how many stars cat has */
	static const MultiString prefixes[26]; /* prefix name list */
	static const MultiString suffixes[26]; /* suffix name list */

public:
	Cat();
	~Cat();

	/**
	 * @brief set cat's clan
	 * @param c clan character
	 * @return true if clan is valid
	 * @note clan character must be one of 't', 'w', 'r', 's'
	 */
	bool setClan(char c);

	/* trivial setters */
	void setHealth(int h);
	void setHunger(int h);
	void setThirst(int t);
	void setStrength(int s);
	void setDefense(int d);
	void setSpeed(int s);
	void setSkill(int s);
	void setMaxVal(int m);
	void setInvincible();
	void setWaterproof();
	void setDiagonal();
	void setMaxStep(int m);

	/**
	 * @brief set cat's name by player's name
	 * @param s player's name
	 * @return true if name is valid
	 * @note first character of name must be a letter
	 */
	bool setName(char *s);

	/* random all attributes */
	void randomAttributes();

	/**
	 * @brief get cat's apprentice name (prefix + "paw")
	 * @return cat's apprentice name
	 */
	std::string getApprenticeName() const;

	/**
	 * @brief get cat's warrior name (prefix + suffix)
	 * @return cat's warrior name
	 * @note only display when win
	 */
	std::string getWarriorName() const;

	/**
	 * @brief render cat's attributes
	 * @param fallback if true, be compatible with Windows console
	 */
	void renderAttributes(bool fallback) const;
	Clan getClan() const;

	/**
	 * @brief use cat's speed to generate step
	 * @return step
	 */
	int generateStep() const;

	/**
	 * @brief call this after a round
	 */
	void loseVal();

	/**
	 * @brief check if cat is dead
	 * display game over and exit if dead
	 */
	void checkDead() const;

	void loseHealth(int h);
	void gainHealth(int h);
	void gainHunger(int h);
	void gainThirst(int h);

	/**
	 * @brief roll dice according to cat's specific value
	 * @param val cat's value type
	 * @return dice result
	 * @note only speed and skill can be used
	 */
	int rollVal(CatVal val) const;
	int rollAttack() const;
	int rollDefense() const;

	/* deal with cat fleeing */
	void flee();
	bool hasFled();
	void resetFled();

	/* deal with cat's stars */
	void incStar();
	int getStars() const;
	bool getDiagonal() const;
	bool getWaterproof() const;
	void rest();
	void drink();
};

#endif