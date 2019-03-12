#include "Pixel.hpp"
#include <cmath>
#include <iostream>
#include "Color.hpp"

Pixel::Pixel() : Point() {
    this->color = Color::BLACK;
}

Pixel::Pixel(float x, float y) : Point(x, y) {
    this->color = Color::WHITE;
}

Pixel::Pixel(float x, float y, unsigned int color) : Point(x, y) {
    this->color = color;
}

unsigned int Pixel::getColor() const {
    return this->color;
}

void Pixel::setColor(unsigned int color) {
    this->color = color;
}

Pixel Pixel::rotation(float pivotX, float pivotY, float theta) {
    float resultX = this->x;
    float resultY = this->y;

    resultX -= pivotX;
    resultY -= pivotY;

    float sinTheta = (float) sin(theta * PI / 180);
    float cosTheta = (float) cos(theta * PI / 180);

    float xnew = resultX * cosTheta - resultY * sinTheta;
    float ynew = resultX * sinTheta + resultY * cosTheta;

    resultX = xnew + pivotX;
    resultY = ynew + pivotY;

    return Pixel(resultX, resultY, this->color);
}

Pixel Pixel::dilated(float pivotX, float pivotY, float scalingConstant) {
    float resultX = scalingConstant * (x - pivotX) + pivotX;
    float resultY = scalingConstant * (y - pivotY) + pivotY;

//    cerr<<"transform "<<pivotX<<" "<<pivotY<<" "<<scalingConstant<<" from "<<x<<" "<<y<<" to "<<resultX<<" "<<resultY<<endl;

    return Pixel(resultX, resultY, this->color);
}

Pixel Pixel::stretchX(float pivotX, float pivotY, float scalingConstant) {
    float resultX = scalingConstant * (x - pivotX) + pivotX;

    return Pixel(resultX, y, this->color);
}

Pixel Pixel::stretchY(float pivotX, float pivotY, float scalingConstant) {
    float resultY = scalingConstant * (y - pivotY) + pivotY;

    return Pixel(x, resultY, this->color);
}
