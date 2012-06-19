#ifndef MAGAME_LINEBYLINE_READER_HPP
#define MAGAME_LINEBYLINE_READER_HPP
#include "StdAfx.h"

class LineByLineReader
{
    typedef std::string T;
    std::istream& input;

    struct iterator {
        typedef std::input_iterator_tag iterator_category;
        typedef T value_type;
        typedef size_t difference_type;
        typedef T* pointer;
        typedef T& reference;

        iterator(std::istream& input, bool at_end);
        void readline();

        bool operator != (iterator const& other) const { return at_end != other.at_end; }
        bool operator == (iterator const& other) const { return !operator!=(other); }
        value_type const& operator*() const { return value; }
        iterator& operator++(int) { return operator++(); }
        iterator& operator++() {
            readline();
            return *this;
        }

        std::istream& input;
        value_type value;
        bool at_end;
    };
public:
    LineByLineReader(std::istream& input) :
        input(input)
    {
    }

    iterator begin() { return iterator(input, false); }
    iterator end() { return iterator(input, true); }
};
#endif // MAGAME_LINEBYLINE_READER_HPP
