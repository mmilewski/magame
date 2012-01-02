#include "StdAfx.h"

#include "Vector2.h"

const Vector2 Vector2::ZERO = Vector2(0, 0);

std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "[" << v.X() << ", " << v.Y() << "]";
    return os;
}
