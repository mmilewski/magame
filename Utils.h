#ifndef __UTILS_H_INCLUDED__
#define __UTILS_H_INCLUDED__
#include <sstream>
#include <string>

/**
 * Zamienia liczbę number na  odpowiadający jej ciąg znaków.
 */
std::string IntToStr(int number);
int StrToInt(const std::string& number);

#endif
