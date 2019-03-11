#ifndef VIEW_HPP
#define VIEW_HPP

#include "Rectangle.hpp"
#include "Object.hpp"

class View {
private:
    Rectangle viewbox;
    Point position;
public:
    View(const Point &position, const Rectangle &box);

    /* Setter */
    void setPosition(const Point &position);
    void setBox(const Rectangle &box);
    /* Getter */
    Point getPos();
    Rectangle getBox();
    Point & getRefPos();
    Rectangle & getRefBox();
    const Point & getConstRefPos() const;
    const Rectangle & getConstRefBox() const;

    int getWidth();
    int getHeight();

    bool isInside(const Object &obj) const;
    bool isInside(const Plane &plane) const;
    bool isInside(const Line &line) const;
    bool isInside(const Point &point) const;
    bool isInside(int x, int y) const;
    bool isInside(int offsetx, int offsety, const Object &obj) const;
    bool isInside(int offsetx, int offsety, const Plane &plane) const;
    bool isInside(int offsetx, int offsety, const Line &line) const;
    bool isInside(int offsetx, int offsety, const Point &point) const;
};


#endif
