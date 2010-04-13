#ifndef __AABB_H__
#define __AABB_H__
#include "StdAfx.h"


// Axis-Aligned Bounding Box
class Aabb {
public:
    Aabb(double min_x, double min_y, double max_x, double max_y) :
        m_min_x(min_x), m_min_y(min_y), m_max_x(max_x), m_max_y(max_y) {

        // można zamienić wartości zamiast sprawdzać asercję, ale
        // błędne przekazanie argumentów mogło być skutkiem
        // błędu podczas tworzenia obiektu (źle podane argumenty)

        assert(m_min_x < m_max_x);
        assert(m_min_y < m_max_y);

        // EDIT: asercje faktycznie przydały się parokrotnie ;-)
    }

    bool Collides(const Aabb& box) const;
    bool IsOver(const Aabb& box) const;
    bool IsUnder(const Aabb& box) const;
    bool IsOnLeftOf(const Aabb& box) const;
    bool IsOnRightOf(const Aabb& box) const;

    double GetMinX() const { return m_min_x; }
    double GetMinY() const { return m_min_y; }
    double GetMaxX() const { return m_max_x; }
    double GetMaxY() const { return m_max_y; }

    Aabb Move(double min_x_offset, double min_y_offset, double max_x_offset, double max_y_offset) {
        return Aabb(m_min_x + min_x_offset, m_min_y + min_y_offset, m_max_x + max_x_offset, m_max_y + max_y_offset);
    }

    friend std::ostream& operator<< (std::ostream& os, const Aabb& box);

private:
    // pozycja lewego dolnego narożnika
    double m_min_x;
    double m_min_y;

    // pozycja prawego górnego narożnika
    double m_max_x;
    double m_max_y;
};

//typedef boost::shared_ptr<Aabb> AabbPtr;

#endif

