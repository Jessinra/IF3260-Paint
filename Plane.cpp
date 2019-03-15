
#include "Plane.hpp"
#include <iostream>

using namespace std;

Plane::Plane(const vector<Line> &lines, unsigned int color, int priority) {
    this->lines = lines;
    this->color = color;
    this->priority = priority;
    this->position = Point(0, 0);
    this->calculate();
}

Plane::Plane(const Point &pos, const vector<Line> &lines, unsigned int color, int priority) {

    this->lines = lines;
    this->color = color;
    this->priority = priority;
    this->position = pos;
    this->calculate();
}


Plane::Plane(const Point &pos, const Plane &ot) {
    this->box = ot.getConstRefBoundedBox();
    this->position = pos;
    this->calculate();
}

void Plane::reverseHorizontal() {
    for (Line &line : this->lines) {
        line.setStartPixel(Pixel(this->getWidth() - line.getStartPixel().getX() - 1, line.getStartPixel().getY(),
                                 line.getStartPixel().getColor()));
        line.setEndPixel(Pixel(this->getWidth() - line.getEndPixel().getX() - 1, line.getEndPixel().getY(),
                               line.getEndPixel().getColor()));
    }
}

void Plane::setColor(unsigned int color) {
    this->color = color;
}

void Plane::setPriority(int priority) {
    this->priority = priority;
}

unsigned int Plane::getColor() const {
    return this->color;
}

int Plane::getPriority() const {
    return this->priority;
}

int Plane::getWidth() const {
    return this->xMax - this->xMin + 1;
}

int Plane::getHeight() const {
    return this->yMax - this->yMin + 1;
}

vector<Line> Plane::getLines() const {
    return this->lines;
}


vector<Line> &Plane::getRefLines() {
    return this->lines;
}

void Plane::selfRotate(float pivotX, float pivotY, float theta) {
    // Polymorphism
}

void Plane::selfDilate(float pivotX, float pivotY, float scalingConstant) {
    // Polymorphism
}

void Plane::calculate() {
//    cerr<<"aku terpanggil"<<" ";
    if (lines.empty()) return;

    this->xMin = min(lines[0].getRefStartPixel().getX(), lines[0].getRefEndPixel().getX());
    this->xMax = max(lines[0].getRefStartPixel().getX(), lines[0].getRefEndPixel().getX());
    this->yMin = min(lines[0].getRefStartPixel().getY(), lines[0].getRefEndPixel().getY());
    this->yMax = max(lines[0].getRefStartPixel().getY(), lines[0].getRefEndPixel().getY());
    for (int i = 1; i < lines.size(); ++i) {
        this->xMin = min(this->xMin, min(lines[i].getRefStartPixel().getX(), lines[i].getRefEndPixel().getX()));
        this->xMax = max(this->xMax, max(lines[i].getRefStartPixel().getX(), lines[i].getRefEndPixel().getX()));
        this->yMin = min(this->yMin, min(lines[i].getRefStartPixel().getY(), lines[i].getRefEndPixel().getY()));
        this->yMax = max(this->yMax, max(lines[i].getRefStartPixel().getY(), lines[i].getRefEndPixel().getY()));
    }
//    cerr<<"aduh "<<xMin<<" "<<yMin<<" "<<xMax<<" "<<yMax<<endl;

    for (Line &line : this->lines) {
        line.setStartPixel(Pixel(line.getStartPixel().getX() - this->xMin, line.getStartPixel().getY() - this->yMin,
                                 line.getStartPixel().getColor()));
        line.setEndPixel(Pixel(line.getEndPixel().getX() - this->xMin, line.getEndPixel().getY() - this->yMin,
                               line.getEndPixel().getColor()));
    }

    this->xMax -= this->xMin;
    this->yMax -= this->yMin;

    this->position.setPoint(this->position.getX() + xMin, this->position.getY() + yMin);

    this->xMin = this->yMin = 0;
}

const vector<Line> &Plane::getConstRefLines() const {
    return this->lines;
}

Point Plane::getLowerRight() const {
    return Point(xMax, yMax);
}

Point Plane::getUpperLeft() const {
    return Point(xMin, yMin);
}

void Plane::setPos(Point position) {
    this->position = position;
}

void Plane::setPos(float x, float y) {
    this->position = Point(x, y);
}


Point Plane::getPos() const {
    return this->position;
}

Rectangle Plane::getBoundedBox() const {
    return box;
}

const Point &Plane::getConstRefPos() const {
    return this->position;
}

Point &Plane::getRefPos() {
    return position;
}

Rectangle &Plane::getRefBoundedBox() {
    return box;
}

const Rectangle &Plane::getConstRefBoundedBox() const {
    return box;
}

Plane::Plane() {
    position.setPoint(0, 0);
    xMin = xMax = yMin = yMax = 0;
}

void Plane::writeToFile(ofstream &fileOutput) {
    fileOutput << dec << this->lines.size() << " ";
    fileOutput << "0x" << hex << this->getColor() << " " << dec;
    fileOutput << this->getPriority() << " ";
    fileOutput << setfill('0') << setw(3) << this->getPos().getX() << " ";
    fileOutput << setfill('0') << setw(3) << this->getPos().getY() << "\n";

    for(Line &line : this->lines) {
        line.writeToFile(fileOutput);
    }

    fileOutput << "\n";
}

