#include "multistring.h"

MultiString::MultiString(std::vector<std::string> strings) : strings(strings)
{
}

MultiString::Language MultiString::currentLanguage;

void MultiString::setLanguage(MultiString::Language lang)
{
	currentLanguage = lang;
}

std::ostream &operator<<(std::ostream &os, const MultiString &ms)
{
	os << ms.strings[MultiString::currentLanguage];
	return os;
}

std::string operator+(const MultiString &ms, const std::string &str)
{
	return ms.strings[MultiString::currentLanguage] + str;
}

std::string operator+(const std::string &str, const MultiString &ms)
{
	return str + ms.strings[MultiString::currentLanguage];
}

MultiString::operator const char *() const
{
	return strings[MultiString::currentLanguage].c_str();
}