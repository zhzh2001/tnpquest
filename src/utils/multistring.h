#ifndef __MULTISTRING_H__
#define __MULTISTRING_H__
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief The MultiString class
 * helper class to display multi-language strings
 */
class MultiString
{
public:
	enum Language
	{
		en, /* English */
		zh	/* Chinese */
	};

private:
	std::vector<std::string> strings; /* strings in different languages */
	static Language currentLanguage;  /* current language */

public:
	/**
	 * @brief create a multi-language string
	 * @param strings strings in different languages
	 */
	MultiString(std::vector<std::string> strings);

	/**
	 * @brief set current language
	 * @param lang language to set
	 */
	static void setLanguage(Language lang);

	/**
	 * @brief ostream friend operator
	 * @param os ostream object
	 * @param ms multi-language string object
	 * @return ostream object
	 */
	friend std::ostream &operator<<(std::ostream &os, const MultiString &ms);

	/**
	 * @brief string friend concat operator
	 * @param ms multi-language string object
	 * @param str string object
	 * @return concatenated string
	 * @note this operator is used to concatenate multi-language string with string
	 */
	friend std::string operator+(const MultiString &ms, const std::string &str);
	friend std::string operator+(const std::string &str, const MultiString &ms);

	/**
	 * @brief conversion operator to char*
	 * @return char*
	 */
	operator const char *() const;
};

#endif