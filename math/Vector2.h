#ifndef MAGAME_VECTOR2_H_INCLUDED
#define MAGAME_VECTOR2_H_INCLUDED
#include "StdAfx.h"

struct Vector2
    : boost::additive<Vector2, boost::multiplicative2<Vector2, double> >
    // : boost::addable< Vector2               // Vector2 + Vector2
    // , boost::subtractable< Vector2          // Vector2 - Vector2
    // , boost::dividable2< Vector2, double    // Vector2 / double
    // , boost::multipliable2< Vector2, double // Vector2 * double, double * Vector2
    //   > > > >
{
    static const Vector2 ZERO;
    Vector2(double x, double y) : x(x), y(y) {}
    double  operator[](int idx) const { return (idx==0?x:y); }
    double& operator[](int idx)       { return (idx==0?x:y); }
    double X() const { return x; }
    double Y() const { return y; }
    void operator+=(const Vector2& other) { x+=other.x; y+=other.y; }
    void operator-=(const Vector2& other) { x-=other.x; y-=other.y; }
    void operator*=(double scalar) { x*=scalar; y*=scalar; }
    void operator/=(double scalar) { x/=scalar; y/=scalar; }
    double Length() const { return std::sqrt(x*x+y*y); }
    Vector2 Scale(double sx, double sy) const { return Vector2(x*sx, y*sy); }
    Vector2 Scale(Vector2 const& scale_xy) const { return Scale(scale_xy.X(), scale_xy.Y()); }
    Vector2 CutToInt() const { return Vector2(static_cast<int>(x), static_cast<int>(y)); }

    template<typename UnaryFunction>
    Vector2 Map(UnaryFunction f) const { return Vector2(f(x), f(y)); }
private:
    double x, y;
};

std::ostream& operator<<(std::ostream& os, const Vector2& v);

#endif // MAGAME_VECTOR2_H_INCLUDED
