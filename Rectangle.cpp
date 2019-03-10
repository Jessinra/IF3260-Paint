#include "Rectangle.hpp"

Rectangle::Rectangle() {
    xMin = xMax = yMin = yMax = 0;
}

Rectangle::Rectangle(float _xMin, float _yMin, float _xMax, float _yMax) {
    xMin = _xMin;
    xMax = _xMax;
    yMin = _yMin;
    yMax = _yMax;
}

Rectangle::Rectangle(const Point &upperleft, const Point &lowerright) {
    xMin = upperleft.getX();
    yMin = upperleft.getY();
    xMax = lowerright.getX();
    yMax = lowerright.getY();
}

void Rectangle::setXMin(float x) {
    xMin = x;
}

void Rectangle::setXMax(float x) {
    xMax = x;
}

void Rectangle::setYMin(float y) {
    yMin = y;
}

void Rectangle::setYMax(float y) {
    yMax = y;
}

float Rectangle::getXMin() const{
    return xMin;
}

float Rectangle::getXMax() const{
    return xMax;
}

float Rectangle::getYMin() const{
    return yMin;
}

float Rectangle::getYMax() const{
    return yMax;
}

