#include "utils.h"

std::string paddingMid(const std::string &str, bool current, int len)
{
	std::string ret = str;
	bool utf8 = str[0] & 0x80; // Chinese character
	int slen = strLen(str);
	int diff = len - slen;
	if (diff < 0)
		return ret;
	int left = diff / 2;
	int right = diff - left;
	return std::string(left, ' ') + ret + std::string(current && utf8 ? right + slen / 2 : right, ' ');
	// why right + slen / 2??? just some dirty magic for Chinese character & Windows
}

int strLen(const std::string &str)
{
	int len = 0;
	for (int i = 0; i < str.size(); i++)
		if (str[i] & 0x80) // Chinese character
		{
			len += 2; // display length = 2
			i += 2;	  // length = 3 in UTF-8
		}
		else
			len++;
	return len;
}