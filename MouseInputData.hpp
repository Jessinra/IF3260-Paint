#ifndef MOUSEINPUTDATA_HPP
#define MOUSEINPUTDATA_HPP

#include "Point.hpp"

enum MouseButtonType{
    LEFT_BUTTON,
    RIGHT_BUTTON,
    UNKNOWN
};

class MouseInputData{
public:
    MouseButtonType buttonType;
    Point position;

    MouseInputData(MouseButtonType buttonType, int x, int y);
    MouseInputData(MouseButtonType buttonType, Point position);
};

#endif