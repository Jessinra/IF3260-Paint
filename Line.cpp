#include "Line.hpp"

Line::Line(Pixel startPixel, Pixel endPixel)
{
    setStartPixel(startPixel);
    setEndPixel(endPixel);
}

void Line::setStartPixel(Pixel startPixel)
{
    this->startPixel = startPixel;
}

void Line::setEndPixel(Pixel endPixel)
{
    this->endPixel = endPixel;
}

Pixel Line::getStartPixel() const
{
    return this->startPixel;
}

Pixel Line::getEndPixel() const
{
    return this->endPixel;
}

unsigned int Line::getStartColor() const
{
    return this->startPixel.getColor();
}

unsigned int Line::getEndColor() const
{
    return this->endPixel.getColor();
}

const Pixel &Line::getRefStartPixel() const
{
    return this->startPixel;
}

const Pixel &Line::getRefEndPixel() const
{
    return this->endPixel;
}

void Line::writeToFile(ofstream &fileOutput) {
    fileOutput << setfill('0') << setw(3) << this->startPixel.getX() << " ";
    fileOutput << setfill('0') << setw(3) << this->startPixel.getY() << " ";
    fileOutput << "0x" << hex << this->startPixel.getColor() << " ";
    fileOutput << setfill('0') << setw(3) << this->endPixel.getX() << " ";
    fileOutput << setfill('0') << setw(3) << this->endPixel.getY() << " ";
    fileOutput << "0x" << hex << this->endPixel.getColor() << "\n";
}