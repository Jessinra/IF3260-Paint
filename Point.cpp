#include "Point.hpp"

Point::Point(float x, float y) : x(x), y(y){}

Point::Point() : x(0), y(0) {}

void Point::setX(float x) {
    this->x = x;
}

void Point::setY(float y) {
    this->y = y;
}

void Point::setPoint(float x, float y){
    this->x = x;
    this->y = y;
}

float Point::getX() const{
    return this->x;
}

float Point::getY() const{
    return this->y;
}
