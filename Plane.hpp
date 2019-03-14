#ifndef PLANE_HPP
#define PLANE_HPP

#include <vector>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include "Line.hpp"
#include "Rectangle.hpp"

using namespace std;

class Plane
{
  protected:
    Point position;
    vector<Line> lines;

    Rectangle box;
    float xMin, yMin, xMax, yMax;

    unsigned int color;
    int priority;

  public:
    Plane();
    Plane(const vector<Line> &lines, unsigned int color, int priority);
    Plane(const Point &pos, const vector<Line> &lines, unsigned int color, int priority);
    Plane(const Point &pos, const Plane &ot);

    void reverseHorizontal();

    void setColor(unsigned int color);
    void setPriority(int priority);
    void calculate();

    unsigned int getColor() const;
    int getPriority() const;
    int getWidth() const;
    int getHeight() const;
    Point getLowerRight() const;
    Point getUpperLeft() const;

    void setPos(Point position);
    void setPos(float x, float y);

    Point getPos() const;
    vector<Line> getLines() const;
    Rectangle getBoundedBox() const;

    Point & getRefPos();
    vector<Line> & getRefLines();
    Rectangle & getRefBoundedBox();

    const Point & getConstRefPos() const;
    const vector<Line> &getConstRefLines() const;
    const Rectangle &getConstRefBoundedBox() const;

    /* For MovableObject polymorphism purpose */
    virtual void selfRotate(float pivotX, float pivotY, float theta);
    virtual void selfDilate(float pivotX, float pivotY, float scalingConstant);

    void writeToFile(ofstream &fileOutput);
};

#endif