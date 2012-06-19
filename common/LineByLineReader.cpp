#include "StdAfx.h"
#include "LineByLineReader.hpp"

LineByLineReader::iterator::iterator(std::istream& input, bool done) :
    input(input), at_end(done) {
    if (!done)
        readline();
}

void LineByLineReader::iterator::readline() {
    if (input) {
        do {
            std::getline(input, value);
        } while(input && (value.empty() || value.at(0) == '#'));
    }
    at_end = at_end || !input;
}
