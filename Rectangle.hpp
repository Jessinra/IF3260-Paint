#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP


#include "Point.hpp"

class Rectangle {
private:
    float xMin, yMin, xMax, yMax;

public:
    Rectangle();
    Rectangle(float _xMin, float _yMin, float _xMax, float _yMax);
    Rectangle(const Point &upperleft, const Point &lowerright);

    void setXMin(float x);
    void setXMax(float x);
    void setYMin(float y);
    void setYMax(float y);

    float getXMin() const;
    float getXMax() const;
    float getYMin() const;
    float getYMax() const;
};


#endif //RECTANGLE_H
