#include "color.h"
#include <ServiceLocator/locator.h>

const Color Color::black{ 0, 0, 0, 255 };
const Color Color::white{ 255, 255, 255, 255 };
const Color Color::grey{ 150, 150 ,150, 255 };
const Color Color::red{ 255, 0, 0, 255 };
const Color Color::green{ 0, 255, 0, 255 };
const Color Color::blue{ 0, 0, 255, 255 };
const Color Color::magenta{ 255, 0, 255, 255 };
const Color Color::yellow{ 255, 255, 0, 255 };
const Color Color::cyan{ 0, 255, 255, 255 };
const Color Color::transparency{ 0, 0, 0, 0 };

void Color::set(int rP, int gP, int bP, int aP)
{
    r = rP;
    g = gP;
    b = bP;
    a = aP;
}

Vector3 Color::toVector() const
{
    return Vector3{ r / 255.0f, g / 255.0f, b / 255.0f };
}

Vector3 Color::ToVector(const Color& color)
{
    return color.toVector();
}

Color Color::HSV(float h, float s, float v)
{
    if (h > 360 || h < 0 || s > 100 || s < 0 || v > 100 || v < 0)
    {
        Locator::getLog().LogMessage_Category("Color: HSV values out of range.", LogCategory::Warning);
        return Color::black;
    }

    s /= 100;
    v /= 100;
    float c = s * v;
    float x = c * static_cast<float>((1.0 - abs(fmod(h / 60.0, 2.0) - 1.0)));
    float m = v - c;
    float r, g, b;
    if (h >= 0 && h < 60)
    {
        r = c, g = x, b = 0;
    }
    else if (h >= 60 && h < 120)
    {
        r = x, g = c, b = 0;
    }
    else if (h >= 120 && h < 180)
    {
        r = 0, g = c, b = x;
    }
    else if (h >= 180 && h < 240)
    {
        r = 0, g = x, b = c;
    }
    else if (h >= 240 && h < 300)
    {
        r = x, g = 0, b = c;
    }
    else
    {
        r = c, g = 0, b = x;
    }
    int R = static_cast<int>(r + m) * 255;
    int G = static_cast<int>(g + m) * 255;
    int B = static_cast<int>(b + m) * 255;
    return Color{ R, G, B, 255 };
}
