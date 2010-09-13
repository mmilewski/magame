#include "StdAfx.h"

#include "Utils.h"

std::string IntToStr(int number) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
}

int StrToInt(const std::string& number) {
    std::istringstream ss(number);;
    int n;
    ss >> n;
    return n;
}


bool StartsWith(const std::string& str, const std::string& prefix) {
    return std::equal(prefix.begin(), prefix.end(), str.begin());
}

bool EndsWith(const std::string& str, const std::string& suffix) {
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

