#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <iomanip>

#include "Plane.hpp"
#include "MoveablePlane.hpp"
using namespace std;

class Object
{
  protected:
    string id;

    Point position;
    vector<MoveablePlane> planes;

    float xMin, yMin, xMax, yMax;

  public:
    Object();
    Object(float x, float y, std::string filename);
    Object(float x, float y, std::string filename, int color);

    void setPos(Point position);
    void setPos(float x, float y);
    void setPlanes(std::string filename);
    void setPlanesColor(int color);

    void addPlane(MoveablePlane plane);

    void calculate();

    vector<MoveablePlane> getPlanes() const;
    Point getPos() const;
    int getWidth() const;
    int getHeight() const;
    string getId() const;
    Point getUpperLeft() const;
    Point getLowerRight() const;

    Point &getRefPos();
    vector<MoveablePlane> & getRefPlanes();

    const vector<MoveablePlane> & getConstRefPlanes() const;
    const Point &getConstRefPos() const;

    void sortPriority();

    void reverseHorizontal(); // TODO
    bool outOfWindow(int height, int width) const;

    void writeToFile(ofstream &fileDetail);
};

#endif