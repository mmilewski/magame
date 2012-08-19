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
        typedef T const& const_reference;

        iterator(std::istream& input, bool at_end);
        void Readline();
        bool IsLineValid(T const& line) const;

        bool operator != (iterator const& other) const { return at_end != other.at_end; }
        bool operator == (iterator const& other) const { return !operator!=(other); }
        const_reference operator*() const { return value; }
        iterator& operator++(int) { return operator++(); }
        iterator& operator++() {
            Readline();
            return *this;
        }

        std::istream& input;
        value_type value;
        bool at_end;
    };

public:
    LineByLineReader(std::istream& input);
    iterator begin() { return iterator(input, false); }
    iterator end() { return iterator(input, true); }
};
#endif // MAGAME_LINEBYLINE_READER_HPP
