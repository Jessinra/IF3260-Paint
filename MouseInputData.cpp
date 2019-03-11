#include "MouseInputData.hpp"

MouseInputData::MouseInputData(MouseButtonType buttonType, int x, int y) {
    this->buttonType = buttonType;
    this->position = Point(x, y);
}

MouseInputData::MouseInputData(MouseButtonType buttonType, Point position) {
    this->buttonType = buttonType;
    this->position = position;
}