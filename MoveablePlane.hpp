#ifndef MOVEABLE_PLANE_HPP
#define MOVEABLE_PLANE_HPP

#include "Plane.hpp"
using namespace std;

class MoveablePlane : public Plane
{
protected:

public:
  MoveablePlane(float x, float y, const vector<Line> &lines, int color, int priority);
  MoveablePlane(float x, float y, const Plane &plane);

  void selfRotate(float pivotX, float pivotY, float theta) override;
  void selfDilate(float pivotX, float pivotY, float scalingConstant) override;
  void selfStretchX(float pivotX, float pivotY, float scalingConstant);
  void selfStretchY(float pivotX, float pivotY, float scalingConstant);
};

#endif