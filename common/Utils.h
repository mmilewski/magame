#ifndef MAGAME_UTILS_H_INCLUDED
#define MAGAME_UTILS_H_INCLUDED
#include "StdAfx.h"


/**
 * Zamienia liczbę number na  odpowiadający jej ciąg znaków.
 */
std::string IntToStr(int number);
int StrToInt(const std::string& number);

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
