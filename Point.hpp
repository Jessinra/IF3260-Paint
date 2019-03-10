#ifndef POINT_H
#define POINT_H

class Point{
protected:
    float x, y;
public:
    Point(float x, float y);
    Point();
    void setX(float x);
    void setY(float y);
    void setPoint(float x, float y);
    float getX() const;
    float getY() const;
};

#endif //POINT_H
