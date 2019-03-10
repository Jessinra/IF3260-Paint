#ifndef PIXEL_HPP
#define PIXEL_HPP

#define PI 3.14159265

#include "Color.hpp"
#include "Point.hpp"

using namespace std;

class Pixel : public Point
{
  private:
    unsigned int color;

  public:
    Pixel();
    Pixel(float x, float y);
    Pixel(float x, float y, unsigned int color);

    unsigned int getColor() const;

    void setColor(unsigned int color);
    
    Pixel rotation(float pivotX, float pivotY, float theta);
    Pixel dilated(float pivotX, float pivotY, float scalingConstant);
};

#endif