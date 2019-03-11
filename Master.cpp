#include "Master.hpp"

int Master::framejump = 1;

Master::Master(int height, int width) {
    setDimension(height, width);

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(-1);
    }

    window = SDL_CreateWindow("Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if(window == NULL){
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        exit(-1);
    }

    screenSurface = SDL_GetWindowSurface(window);

    setScreensize();
    setFrameBufferPointer();
}

Master::~Master() {
    free(this->buffer);
    SDL_DestroyWindow( window );
    SDL_Quit();
}

void Master::setDimension(int height, int width) {
    this->xstart = 0;
    this->ystart = 0;
    this->yend = height;
    this->xend = width;
}

void Master::setScreensize() {
    // Figure out the size of the screen in bytes
    this->screensize = screenSurface->h * screenSurface->pitch;
}

void Master::setFrameBufferPointer() {
    this->buffer = (char *) malloc(this->screensize);
}

bool Master::isInsideWindow(int x, int y) {
    return x >= xstart && x < xend && y >= ystart && y < yend;
}

int Master::getLocation(int x, int y){
    return x * sizeof(unsigned int) + y * screenSurface->pitch;
}

void Master::assignColor(int x, int y, unsigned int color) {
    int location = getLocation(x, y);
    if (isInsideWindow(x, y) && *((unsigned int *) (buffer + location)) == 0) {
        *((unsigned int *) (buffer + location)) = color;
    }
}

unsigned int Master::frameColor(int x, int y) {
    if (isInsideWindow(x, y) ) {
        int location = getLocation(x, y);
        return *((unsigned int *) (buffer+ location));
    }
}

void Master::copyColor(int xTarget, int yTarget, int xSource, int ySource) {
    if (isInsideWindow(xTarget, yTarget) && isInsideWindow(xSource, ySource)) {
        int location1 = getLocation(xTarget, yTarget);
        int location2 = getLocation(xSource, ySource);
        *((unsigned int *) (buffer+ location1)) = *((unsigned int *) buffer + location2);
    }
}

void Master::assignColorBuffer(vector<vector<unsigned int>> &buffer, int x, int y, unsigned int color) {
    if (y < buffer.size() && x < buffer[0].size()) {
        buffer[y][x] = color;
    }
}

void Master::clearWindow() {
    memset(buffer, 0, this->screensize);
}

void Master::clearWindow(unsigned int color) {
    for (int y = ystart; y < yend; y++) {
        for (int x = xstart; x < xend; x++) {
            assignColor(x, y, color);
        }
    }
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
    SDL_LockSurface(screenSurface);
    SDL_memcpy(screenSurface->pixels, buffer, screenSurface->h * screenSurface->pitch);
    SDL_UnlockSurface(screenSurface);
    SDL_UpdateWindowSurface( window );
}

void Master::assignColor(const View &view, int x, int y, unsigned int color) {
    int location = getLocation(x + view.getConstRefPos().getX(), y + view.getConstRefPos().getY());
    if (view.isInside(x, y) && *((unsigned int *) (buffer + location)) == 0) {
        *((unsigned int *) (buffer + location)) = color;
    }
}

void Master::drawLine(const View &view, int positionX, int positionY, const Line &line) {
    // Bresenham's line algorithm with gradient coloring

    // Position section
    int xStart = line.getStartPixel().getX() + positionX;
    int yStart = line.getStartPixel().getY() + positionY;
    int xEnd = line.getEndPixel().getX() + positionX;
    int yEnd = line.getEndPixel().getY() + positionY;

//    if(max(xStart, xEnd) < view.getXMin() || min(xStart, xEnd) >= view.getXMax())
//        return;
//    if(max(yStart, yEnd) < view.getXMin() || min(yStart, yEnd) >= view.getYMax())
//        return;

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

void Master::drawPlane(const View &view, int xStart, int yStart, const Plane &plane) {
    for (const Line &line : plane.getConstRefLines()) {
        if(view.isInside(xStart, yStart, line)) {
            drawLine(view, xStart, yStart, line);
        }
    }
}

void Master::drawSolidPlane(const View &view, int xStart, int yStart, const Plane &plane) {
    vector<Line> planeFillerLines = this->planeFiller.getPlaneFillerLines(plane);

    for (const Line &line : planeFillerLines) {
        if(view.isInside(xStart, yStart, line)){
            drawLine(view, xStart, yStart, line);
        }
    }
}

void Master::drawObject(const View &view, const Object &object) {
    for (const MoveablePlane &plane : object.getConstRefPlanes()) {
        if(view.isInside(object.getConstRefPos().getX(), object.getConstRefPos().getY(), plane)){
            drawPlane(view, object.getConstRefPos().getX() + plane.getConstRefPos().getX(),
                      object.getConstRefPos().getY() + plane.getConstRefPos().getY(), plane);
        }
    }
}

void Master::drawSolidObject(const View &view, const Object &object) {
    for (const MoveablePlane &plane : object.getConstRefPlanes()) {
        if(view.isInside(object.getConstRefPos().getX(), object.getConstRefPos().getY(), plane)){
            drawPlane(view, object.getConstRefPos().getX() + plane.getConstRefPos().getX(), object.getConstRefPos().getY() + plane.getConstRefPos().getY(), plane);
            drawSolidPlane(view, object.getConstRefPos().getX() + plane.getConstRefPos().getX(),
                           object.getConstRefPos().getY() + plane.getConstRefPos().getY(), plane);
        }
    }
}

