// header class for Color
#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <cmath>

class Color {
public:
    float r, g, b;

    Color(float r = 0.0, float g = 0.0, float b = 0.0) : r(r), g(g), b(b) {}

    Color operator+(const Color& c) const { return Color(r + c.r, g + c.g, b + c.b); }
    Color operator-(const Color& c) const { return Color(r - c.r, g - c.g, b - c.b); }
    Color operator*(float f) const { return Color(r * f, g * f, b * f); }
    Color operator*(const Color& c) const { return Color(r * c.r, g * c.g, b * c.b); }

    Color& operator+=(const Color& c) { return *this = *this + c; }
    Color& operator-=(const Color& c) { return *this = *this - c; }
    Color& operator*=(float f) { return *this = *this * f; }
    Color& operator*=(const Color& c) { return *this = *this * c; }

    Color& clamp() {
        r = std::min(1.0f, std::max(0.0f, r));
        g = std::min(1.0f, std::max(0.0f, g));
        b = std::min(1.0f, std::max(0.0f, b));
        return *this;
    }
};

#endif