#include "StdAfx.h"

#include "Aabb.h"

// czy prostokąty się przecinają
bool Aabb::Collides(const Aabb & box) const {
    if (m_min_x > box.m_max_x || m_max_x < box.m_min_x || m_min_y > box.m_max_y || m_max_y < box.m_min_y) {
        return false;
    }
    return true;
}

// czy this jest powyżej box
bool Aabb::IsOver(const Aabb & box) const {
    return Collides(box) && m_min_y < box.m_max_y && box.m_max_y < m_max_y;
}

// czy this jest poniżej box
bool Aabb::IsUnder(const Aabb & box) const {
    // czyli czy box jest powyżej this
    return box.IsOver(*this);
}

// czy this jest po lewej stronie box'a
bool Aabb::IsOnLeftOf(const Aabb & box) const {
    return Collides(box) && m_min_x < box.m_min_x && box.m_min_x < m_max_x;
}

// czy this jest po prawej stronie box'a
bool Aabb::IsOnRightOf(const Aabb & box) const {
    return box.IsOnLeftOf(*this);
}

std::ostream& operator<<(std::ostream& os, const Aabb& box) {
    os << "[ " << box.GetMinX() << ", " << box.GetMinY() << "; " << box.GetMaxX() << ", " << box.GetMaxY() << " ] ";
    return os;
}
