#ifndef __UTILS_H_INCLUDED__
#define __UTILS_H_INCLUDED__
#include "StdAfx.h"



/**
 * Zamienia liczbę number na  odpowiadający jej ciąg znaków.
 */
std::string IntToStr(int number);
int StrToInt(const std::string& number);

/**
 * Sprawdza czy str zaczyna się (kończy się) wskazanym prefiksem (sufiksem).
 */
bool StartsWith(const std::string& str, const std::string& prefix);
bool EndsWith(const std::string& str, const std::string& suffix);


#endif
