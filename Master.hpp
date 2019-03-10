#ifndef MASTER_HPP
#define MASTER_HPP

#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "Line.hpp"
#include "Plane.hpp"
#include "PlaneFiller.hpp"
#include "Pixel.hpp"
#include "Object.hpp"
#include "Rectangle.hpp"

using namespace std;

class Master
{
  protected:
  
    /* unchangeable */
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    int fbfd;
    int screensize;
    char *fbp, *buffer;
    int xmultiplier, ymultiplier;
    int xadder, yadder;

    /* changeable */
    int xstart, xend;
    int ystart, yend;
    static int framejump;

    /* Component */
    PlaneFiller planeFiller;

    /* some unused attribute by end of refactoring #2 */
    // int height; Not used
    // int width; Not used
    // int offsetcycle; Not used

    void setDimension(int height, int width);
    void setFbfd();
    void setVinfo();
    void setFinfo();
    void setScreensize();
    void setFrameBufferPointer();
    void setXMultiplier();
    void setXAdder();
    void setYMultiplier();
    void setYAdder();

  public:
    Master(int height, int width);
    ~Master();
    void displayVinfo();
    void flush();

    bool isInsideWindow(int x, int y);
    void assignColor(int x, int y, unsigned int color);
    unsigned int frameColor(int , int);
    void copyColor(int xTarget, int yTarget, int xSource, int ySource);
    static void assignColorBuffer(vector<vector<unsigned int>> &buffer, int x, int y, unsigned int color);
    void clearWindow();
    void clearWindow(unsigned int color);
    void moveWindowUp();

    void draw(int xStart, int yStart, int **img, int height, int width);
    void draw(int xStart, int yStart, const vector<vector<unsigned int>> &img);

    void drawPixel(int xStart, int yStart, const Pixel &pixel);
    void drawPixels(int xStart, int yStart, vector<Pixel> pixels);
    void drawLine(int positionX, int positionY, const Line &line);

    void drawPlane(int xStart, int yStart, const Plane &plane);
    void drawSolidPlane(int xStart, int yStart, const Plane &plane);

    void drawObject(const Object &object);
    void drawSolidObject(const Object &object);

    void assignColor(const Rectangle &view, int x, int y, unsigned int color);
    void drawLine(const Rectangle &view, int positionX, int positionY, const Line &line);

    void drawPlane(const Rectangle &view, int xStart, int yStart, const Plane &plane);
    void drawSolidPlane(const Rectangle &view, int xStart, int yStart, const Plane &plane);

    void drawObject(const Rectangle &view, const Object &object);
    void drawSolidObject(const Rectangle &view, const Object &object);
};

#endif