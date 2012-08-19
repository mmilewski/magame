#include "StdAfx.h"
#include "LineByLineReader.hpp"

LineByLineReader::LineByLineReader(std::istream& input) :
    input(input)
{
}

LineByLineReader::iterator::iterator(std::istream& input, bool done) :
    input(input), at_end(done) {
    if (!done) {
        Readline();
    }
}

bool LineByLineReader::iterator::IsLineValid(T const& line) const
{
    return !(line.empty() || line.at(0) == '#');
}

void LineByLineReader::iterator::Readline() {
    if (input) {
        do {
            std::getline(input, value);
        } while(input && (!IsLineValid(value)));
    }
    at_end = at_end || !input;
}
