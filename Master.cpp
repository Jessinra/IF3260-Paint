
#include "Master.hpp"

int Master::framejump = 1;

Master::Master(int height, int width) {
    setDimension(height, width);
    setFbfd();
    setVinfo();
    setFinfo();
    setScreensize();
    setFrameBufferPointer();
    setXMultiplier();
    setXAdder();
    setYMultiplier();
    setYAdder();

    displayVinfo();
}

Master::~Master() {
    free(this->buffer);
}

void Master::setDimension(int height, int width) {
    this->xstart = 0;
    this->ystart = 0;
    this->yend = height;
    this->xend = width;
}

void Master::setFbfd() {
    // Open the file for reading and writing

    int fbfd = open("/dev/fb0", O_RDWR);
    if (!fbfd) {
        printf("Error: cannot open framebuffer device.\n");
        exit(1);
    }

    printf("The framebuffer device was opened successfully.\n");
    this->fbfd = fbfd;
}

void Master::setVinfo() {
    // Get fixed screen information
    if (ioctl(this->fbfd, FBIOGET_FSCREENINFO, &this->finfo)) {
        printf("Error reading fixed information.\n");
        exit(2);
    }
}

void Master::setFinfo() {
    // Get variable screen information
    if (ioctl(this->fbfd, FBIOGET_VSCREENINFO, &this->vinfo)) {
        printf("Error reading variable information.\n");
        exit(3);
    }
}

void Master::setScreensize() {
    // Figure out the size of the screen in bytes
    this->screensize = this->vinfo.xres * this->vinfo.yres * this->vinfo.bits_per_pixel / 8;
}

void Master::setFrameBufferPointer() {
    // Map the device to memory
    this->fbp = (char *) mmap(0, this->screensize, PROT_READ | PROT_WRITE, MAP_SHARED, this->fbfd, 0);
    this->buffer = (char *) malloc(this->screensize);
    if ((long int) this->fbp == -1) {
        printf("Error: failed to map framebuffer device to memory.\n");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");
}

void Master::setXAdder() {
    this->xadder = (this->vinfo.xoffset) * this->xmultiplier;
}

void Master::setXMultiplier() {
    this->xmultiplier = (this->vinfo.bits_per_pixel / 8);
}

void Master::setYAdder() {
    this->yadder = this->vinfo.yoffset * this->finfo.line_length;
}

void Master::setYMultiplier() {
    this->ymultiplier = this->finfo.line_length;
}

void Master::displayVinfo() {
    printf("%dx%d, %dbpp\n", this->vinfo.xres, this->vinfo.yres, this->vinfo.bits_per_pixel);
}

bool Master::isInsideWindow(int x, int y) {
    return x >= xstart && x < xend && y >= ystart && y < yend;
}

void Master::assignColor(int x, int y, unsigned int color) {
    int location = x * xmultiplier + xadder + y * ymultiplier + yadder;
    if (isInsideWindow(x, y) && *((unsigned int *) (buffer + location)) == 0) {
        *((unsigned int *) (buffer + location)) = color;
    }
}

unsigned int Master::frameColor(int x, int y) {
    if (isInsideWindow(x, y)) {
        int location = x * xmultiplier + xadder + y * ymultiplier + yadder;
        return *((unsigned int *) (buffer+ location));
    }
}

void Master::copyColor(int xTarget, int yTarget, int xSource, int ySource) {
    if (isInsideWindow(xTarget, yTarget) && isInsideWindow(xSource, ySource)) {
        int location1 = xTarget * xmultiplier + xadder + yTarget * ymultiplier + yadder;
        int location2 = xSource * xmultiplier + xadder + ySource * ymultiplier + yadder;
        *((unsigned int *) (buffer+ location1)) = *((unsigned int *) buffer + location2);
    }
}

void Master::assignColorBuffer(vector<vector<unsigned int>> &buffer, int x, int y, unsigned int color) {
    if (y < buffer.size() && x < buffer[0].size()) {
        buffer[y][x] = color;
    }
}

void Master::clearWindow() {
    memset(buffer, 0, (yend * ymultiplier + yadder));
}

void Master::clearWindow(unsigned int color) {
    for (int y = ystart; y < yend; y++) {
        for (int x = xstart; x < xend; x++) {
            assignColor(x, y, color);
        }
    }
}

void Master::moveWindowUp() {
    for (int y = ystart; y < yend - framejump; y += framejump) {
        int location1 = xadder + y * ymultiplier + yadder;
        int location2 = location1 + framejump * ymultiplier;
        memcpy(buffer + location1, buffer + location2, framejump * (1 + vinfo.yoffset) * finfo.line_length);
    }
    memset(buffer + (yend - framejump) * ymultiplier + yadder, 0, framejump * (ymultiplier + yadder));
}

void Master::draw(int xStart, int yStart, int **img, int height, int width) {
    for (int y = max(0, -yStart); y < height; y++) {
        if (y + yStart >= this->yend) {
            break;
        }

        for (int x = max(0, -xStart); x < width; x++) {
            if (x + xStart >= this->xend) {
                break;
            }

            assignColor(x + xStart, y + yStart, img[y][x]);
        }
    }
}

void Master::draw(int xStart, int yStart, const vector<vector<unsigned int>> &img) {
    for (int y = max(0, -yStart); y < (int) img.size(); y++) {
        if (y + yStart >= this->yend) {
            break;
        }

        for (int x = max(0, -xStart); x < img[y].size(); x++) {
            if (x + xStart >= this->xend) {
                break;
            }

            assignColor(x + xStart, y + yStart, img[y][x]);
        }
    }
}

void Master::drawPixel(int xStart, int yStart, const Pixel &pixel) {
    int x = xStart + pixel.getX();
    int y = yStart + pixel.getY();
    assignColor(x, y, pixel.getColor());
}

void Master::drawPixels(int xStart, int yStart, vector<Pixel> pixels) {
    for (Pixel &p : pixels) {
        drawPixel(xStart, yStart, p);
    }
}

void Master::drawLine(int positionX, int positionY, const Line &line) {
    // Bresenham's line algorithm with gradient coloring

    // Position section
    int xStart = line.getStartPixel().getX() + positionX;
    int yStart = line.getStartPixel().getY() + positionY;
    int xEnd = line.getEndPixel().getX() + positionX;
    int yEnd = line.getEndPixel().getY() + positionY;

    if(max(xStart, xEnd) < 0 || min(xStart, xEnd) >= xend)
        return;
    if(max(yStart, yEnd) < 0 || min(yStart, yEnd) >= yend)
        return;

    // Color section
    int colorStart = line.getStartPixel().getColor();
    int colorEnd = line.getEndPixel().getColor();

    // Setup Const
    const float deltaX = xEnd - xStart;
    const float deltaY = yEnd - yStart;

    const float deltaRed = ((colorEnd & 0xff0000) - (colorStart & 0xff0000)) >> 16;
    const float deltaGreen = ((colorEnd & 0xff00) - (colorStart & 0xff00)) >> 8;
    const float deltaBlue = ((colorEnd & 0xff) - (colorStart & 0xff));

    const float manhattanDist = fabs(deltaX) + fabs(deltaY) + 1;

    const float redStep = deltaRed / manhattanDist;
    const float greenStep = deltaGreen / manhattanDist;
    const float blueStep = deltaBlue / manhattanDist;

    const int xStep = deltaX >= 0 ? 1 : -1;
    const int yStep = deltaY >= 0 ? 1 : -1;

    float red = (colorStart & 0xff0000) >> 16;
    float green = (colorStart & 0xff00) >> 8;
    float blue = colorStart & 0xff;

    if (xStart == xEnd) {
        for (int y = yStart; y != yEnd + yStep; y += yStep) {
            unsigned int color = ((unsigned int) floor(red) << 16) + ((unsigned int) floor(green) << 8) +
                                 ((unsigned int) floor(blue));
            if (frameColor(xStart, y) == 0) {
                assignColor(xStart, y, color);
            }

            red += redStep;
            green += greenStep;
            blue += blueStep;
        }
        return;
    }

    int y = yStart;
    const float deltaErr = fabs(deltaY / deltaX);
    float error = 0;
    for (int x = xStart; x != xEnd + xStep;) {
        unsigned int color =
                ((unsigned int) floor(red) << 16) + ((unsigned int) floor(green) << 8) + ((unsigned int) floor(blue));
        if (frameColor(x, y) == 0) {
            assignColor(x, y, color);
        }

        if (error >= 0.5) {
            y += yStep;
            error -= 1;
        } else {
            error += deltaErr;
            x += xStep;
            if (error >= 0.5) {
                y += yStep;
                error -= 1;
            }
        }

        red += redStep;
        green += greenStep;
        blue += blueStep;
    }
}

void Master::drawPlane(int xStart, int yStart, const Plane &plane) {
    for (const Line &line : plane.getConstRefLines()) {
        drawLine(xStart, yStart, line);
    }
}

void Master::drawSolidPlane(int xStart, int yStart, const Plane &plane) {
    vector<Line> planeFillerLines = this->planeFiller.getPlaneFillerLines(plane);

    for (const Line &line : planeFillerLines) {
        drawLine(xStart, yStart, line);
    }
}

void Master::drawObject(const Object &object) {
    for (const MoveablePlane &plane : object.getConstRefPlanes()) {
        drawPlane(object.getConstRefPos().getX() + plane.getConstRefPos().getX(),
                object.getConstRefPos().getY() + plane.getConstRefPos().getY(), plane);
    }
}

void Master::drawSolidObject(const Object &object) {
    for (const MoveablePlane &plane : object.getConstRefPlanes()) {
        drawSolidPlane(object.getConstRefPos().getX() + plane.getConstRefPos().getX(),
                object.getConstRefPos().getY() + plane.getConstRefPos().getY(), plane);
    }
}

void Master::flush() {
    memcpy(fbp, buffer, (yend * ymultiplier + yadder));
}

void Master::assignColor(const Rectangle &view, int x, int y, unsigned int color) {
    int location = x * xmultiplier + xadder + y * ymultiplier + yadder;
    if ((x>=view.getXMin() && x<view.getXMax() && y>=view.getYMin() && y<view.getYMax()) && *((unsigned int *) (buffer + location)) == 0) {
        *((unsigned int *) (buffer + location)) = color;
    }
}

void Master::drawLine(const Rectangle &view, int positionX, int positionY, const Line &line) {
    // Bresenham's line algorithm with gradient coloring

    // Position section
    int xStart = line.getStartPixel().getX() + positionX;
    int yStart = line.getStartPixel().getY() + positionY;
    int xEnd = line.getEndPixel().getX() + positionX;
    int yEnd = line.getEndPixel().getY() + positionY;

    if(max(xStart, xEnd) < view.getXMin() || min(xStart, xEnd) >= view.getXMax())
        return;
    if(max(yStart, yEnd) < view.getXMin() || min(yStart, yEnd) >= view.getYMax())
        return;

    // Color section
    int colorStart = line.getStartPixel().getColor();
    int colorEnd = line.getEndPixel().getColor();

    // Setup Const
    const float deltaX = xEnd - xStart;
    const float deltaY = yEnd - yStart;

    const float deltaRed = ((colorEnd & 0xff0000) - (colorStart & 0xff0000)) >> 16;
    const float deltaGreen = ((colorEnd & 0xff00) - (colorStart & 0xff00)) >> 8;
    const float deltaBlue = ((colorEnd & 0xff) - (colorStart & 0xff));

    const float manhattanDist = fabs(deltaX) + fabs(deltaY) + 1;

    const float redStep = deltaRed / manhattanDist;
    const float greenStep = deltaGreen / manhattanDist;
    const float blueStep = deltaBlue / manhattanDist;

    const int xStep = deltaX >= 0 ? 1 : -1;
    const int yStep = deltaY >= 0 ? 1 : -1;

    float red = (colorStart & 0xff0000) >> 16;
    float green = (colorStart & 0xff00) >> 8;
    float blue = colorStart & 0xff;

    if (xStart == xEnd) {
        for (int y = yStart; y != yEnd + yStep; y += yStep) {
            unsigned int color = ((unsigned int) floor(red) << 16) + ((unsigned int) floor(green) << 8) +
                                 ((unsigned int) floor(blue));
            if (frameColor(xStart, y) == 0) {
                assignColor(view, xStart, y, color);
            }

            red += redStep;
            green += greenStep;
            blue += blueStep;
        }
        return;
    }

    int y = yStart;
    const float deltaErr = fabs(deltaY / deltaX);
    float error = 0;
    for (int x = xStart; x != xEnd + xStep;) {
        unsigned int color =
                ((unsigned int) floor(red) << 16) + ((unsigned int) floor(green) << 8) + ((unsigned int) floor(blue));
        if (frameColor(x, y) == 0) {
            assignColor(view, x, y, color);
        }

        if (error >= 0.5) {
            y += yStep;
            error -= 1;
        } else {
            error += deltaErr;
            x += xStep;
            if (error >= 0.5) {
                y += yStep;
                error -= 1;
            }
        }

        red += redStep;
        green += greenStep;
        blue += blueStep;
    }
}

void Master::drawPlane(const Rectangle &view, int xStart, int yStart, const Plane &plane) {
    for (const Line &line : plane.getConstRefLines()) {
        drawLine(view, xStart, yStart, line);
    }
}

void Master::drawSolidPlane(const Rectangle &view, int xStart, int yStart, const Plane &plane) {
    vector<Line> planeFillerLines = this->planeFiller.getPlaneFillerLines(plane);

    for (const Line &line : planeFillerLines) {
        drawLine(view, xStart, yStart, line);
    }
}

void Master::drawObject(const Rectangle &view, const Object &object) {
    for (const MoveablePlane &plane : object.getConstRefPlanes()) {
        if(object.getConstRefPos().getX() >= view.getXMax() || object.getConstRefPos().getY() >= view.getYMax()
           || object.getConstRefPos().getX() + plane.getLowerRight().getX() < view.getXMin() || object.getConstRefPos().getY() + plane.getLowerRight().getY() < view.getYMin()) continue;
        drawPlane(view, object.getConstRefPos().getX() + plane.getConstRefPos().getX(),
                object.getConstRefPos().getY() + plane.getConstRefPos().getY(), plane);
    }
}

void Master::drawSolidObject(const Rectangle &view, const Object &object) {
    for (const MoveablePlane &plane : object.getConstRefPlanes()) {
        if(object.getConstRefPos().getX() + plane.getConstRefPos().getX() >= view.getXMax() || object.getConstRefPos().getY() + plane.getConstRefPos().getY() >= view.getYMax()
           || object.getConstRefPos().getX() + plane.getConstRefPos().getX() + plane.getLowerRight().getX() < view.getXMin() || object.getConstRefPos().getY() + plane.getConstRefPos().getY() + plane.getLowerRight().getY() < view.getYMin()) continue;
        drawPlane(view, object.getConstRefPos().getX() + plane.getConstRefPos().getX(), object.getConstRefPos().getY() + plane.getConstRefPos().getY(), plane);
        drawSolidPlane(view, object.getConstRefPos().getX() + plane.getConstRefPos().getX(),
                object.getConstRefPos().getY() + plane.getConstRefPos().getY(), plane);
    }
}

