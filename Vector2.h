#ifndef __VECTOR2_H_INCLUDED__
#define __VECTOR2_H_INCLUDED__

struct Vector2
    : boost::arithmetic<Vector2>
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
    Vector2 operator+=(const Vector2& other) { return Vector2(x+other.x, y+other.y); }
    Vector2 operator-=(const Vector2& other) { return Vector2(x-other.x, y-other.y); }
    Vector2 operator*=(double scalar) { return Vector2(x*scalar, y*scalar); }
    Vector2 operator/=(double scalar) { return Vector2(x/scalar, y/scalar); }
    double Length() const { return std::sqrt(x*x+y*y); }
private:
    double x, y;
};

#endif // __VECTOR2_H_INCLUDED__
