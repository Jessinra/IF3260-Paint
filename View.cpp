#include "View.hpp"

View::View(const Point &position, const Rectangle &box) {
    this->position = position;
    this->viewbox = box;
}

void View::setPosition(const Point &position) {
    this->position = position;
}

void View::setBox(const Rectangle &box) {
    this->viewbox = box;
}

Point View::getPos() {
    return position;
}

Rectangle View::getBox() {
    return viewbox;
}

Point &View::getRefPos() {
    return position;
}

Rectangle &View::getRefBox() {
    return viewbox;
}

const Point &View::getConstRefPos() const {
    return position;
}

const Rectangle &View::getConstRefBox() const {
    return viewbox;
}

int View::getWidth() {
    return viewbox.getXMax() - viewbox.getXMin();
}

int View::getHeight() {
    return viewbox.getYMax() - viewbox.getYMin();
}

bool View::isInside(const Object &obj) const{
    return isInside(0, 0, obj);
}

bool View::isInside(const Plane &plane) const{
    return isInside(0, 0, plane);
}

bool View::isInside(const Line &line) const{
    return isInside(0, 0, line);
}

bool View::isInside(const Point &point) const{
    return isInside(point.getX(), point.getY());
}

bool View::isInside(int offsetx, int offsety, const Object &obj) const{
    offsetx += obj.getConstRefPos().getX();
    offsety += obj.getConstRefPos().getY();
    return !(offsetx >= viewbox.getXMax() || offsetx + obj.getLowerRight().getX() < viewbox.getXMin() ||
            offsety >= viewbox.getYMax() || offsety + obj.getLowerRight().getY() < viewbox.getYMin());
}

bool View::isInside(int offsetx, int offsety, const Plane &plane) const{
    offsetx += plane.getConstRefPos().getX();
    offsety += plane.getConstRefPos().getY();
    return !(offsetx >= viewbox.getXMax() || offsetx + plane.getLowerRight().getX() < viewbox.getXMin() ||
             offsety >= viewbox.getYMax() || offsety + plane.getLowerRight().getY() < viewbox.getYMin());
}

bool View::isInside(int offsetx, int offsety, const Line &line) const{
    int minX = min(line.getRefStartPixel().getX(), line.getRefEndPixel().getX());
    int maxX = max(line.getRefStartPixel().getX(), line.getRefEndPixel().getX());
    int minY = min(line.getRefStartPixel().getY(), line.getRefEndPixel().getY());
    int maxY = max(line.getRefStartPixel().getY(), line.getRefEndPixel().getY());
    return !(offsetx + minX >= viewbox.getXMax() || offsetx +maxX < viewbox.getXMin() ||
             offsety + minY >= viewbox.getYMax() || offsety + maxY < viewbox.getYMin());
}

bool View::isInside(int offsetx, int offsety, const Point &point) const{
    offsetx += point.getX();
    offsety += point.getY();
    return isInside(offsetx, offsety);
}

bool View::isInside(int x, int y) const {
    return x >= viewbox.getXMin() && x <= viewbox.getXMax() && y >= viewbox.getYMin() && y <= viewbox.getYMax();
}
