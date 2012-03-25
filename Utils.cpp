#include "StdAfx.h"

#include "Utils.h"

std::string IntToStr(int number) {
    return boost::lexical_cast<std::string>(number);
}

int StrToInt(const std::string& number) {
  return boost::lexical_cast<int>(number);
}


bool StartsWith(const std::string& str, const std::string& prefix) {
    return std::equal(prefix.begin(), prefix.end(), str.begin());
}

bool EndsWith(const std::string& str, const std::string& suffix) {
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

