#ifndef __UTILS_H__
#define __UTILS_H__
#include <string>

/**
 * @brief padding string with spaces to centered
 * @param str string to be padded
 * @param current true if string is selected (dirty magic here)
 * @param len desired length of string
 * @return padded string
 */
std::string paddingMid(const std::string &str, bool current, int len = 8);

/**
 * @brief string display length with respect to UTF-8
 * @param str string to be measured
 * @return string display length
 * @note this function is used to calculate string length in console
 */
int strLen(const std::string &str);
#endif