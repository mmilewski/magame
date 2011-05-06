#ifndef __VECTOR2_H_INCLUDED__
#define __VECTOR2_H_INCLUDED__

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
    Vector2 scale(double sx, double sy) { return Vector2(x*sx, y*sy); }
private:
    double x, y;
};

std::ostream& operator<<(std::ostream& os, const Vector2& v);

#endif // __VECTOR2_H_INCLUDED__
