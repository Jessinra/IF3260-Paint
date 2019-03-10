#ifndef LINE_HPP
#define LINE_HPP

#include <cmath>

#include "Pixel.hpp"
using namespace std;

class Line
{
  protected:
    Pixel startPixel;
    Pixel endPixel;

  public:
    Line(Pixel startPixel, Pixel endPixel);

    void setStartPixel(Pixel startPixel);
    void setEndPixel(Pixel endPixel);
    
    Pixel getStartPixel() const;
    Pixel getEndPixel() const;
    unsigned int getStartColor() const;
    unsigned int getEndColor() const;
    const Pixel &getRefStartPixel() const;
    const Pixel &getRefEndPixel() const;
};

#endif