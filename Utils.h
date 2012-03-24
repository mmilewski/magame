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

/**
 * Sprawdza czy element należy do kontenera.
 * @todo Funkcja mogłaby działać także dla kontenerów asocjacyjnych (mają one metodę count())
 */
template <class Container, class Value>
bool has(Container c, Value v) {
    return boost::find(c, v) != c.end();
}

template <class Container, class Value>
bool hasnt(Container c, Value v)
{
    return !has(c, v);
}

#endif
