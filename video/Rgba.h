#ifndef MAGAME_RGBA_H_INCLUDED
#define MAGAME_RGBA_H_INCLUDED

struct Rgba {
    Rgba(double _r, double _g, double _b, double _a = 1.0)
      : r(_r), g(_g), b(_b), a(_a)
    {
    }

    double r, g, b, a;    // red, green, blue, alpha?
};

#endif // MAGAME_RGBA_H_INCLUDED
