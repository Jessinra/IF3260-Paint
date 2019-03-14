
#include "Object.hpp"

string gen_random(const int len) {
    string s = "";
    
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return s;
}

Object::Object()
{
    xMin = xMax = yMin = yMax = 0;
}

Object::Object(float x, float y, std::string filename)
{
    setPos(x, y);
    setPlanes(filename);

    this->id = gen_random(10);

    calculate();
    sortPriority();
}

Object::Object(float x, float y, std::string filename, int planeColor)
{
    setPos(x, y);
    setPlanes(filename);
    setPlanesColor(planeColor);

    this->id = gen_random(10);

    calculate();
    sortPriority();
}

void Object::setPlanes(std::string filename){
        // initialize lines
    ifstream inFile;
    inFile.open(filename);

    if (!inFile)
    {
        cout << "\nError opening file.\n";
        exit(1);
    }

    int nLine;
    int priority;
    int xStart, yStart, xEnd, yEnd;
    int planeColor;
    unsigned int colorStart, colorEnd;
    int offsetX, offsetY;

    int nPlane;
    inFile >> nPlane;
    for (int i = 0; i < nPlane; ++i)
    {
        inFile >> dec >> nLine;
        inFile >> hex >> planeColor;
        inFile >> dec >> priority;
        inFile >> offsetX;
        inFile >> offsetY;

        vector<Line> lines;
        for (int j = 0; j < nLine; ++j)
        {

            inFile >> dec >> xStart;
            inFile >> yStart;
            inFile >> hex >> colorStart;
            inFile >> dec >> xEnd;
            inFile >> yEnd;
            inFile >> hex >> colorEnd;

            Pixel startpx = Pixel(xStart, yStart, colorStart);
            Pixel endpx = Pixel(xEnd, yEnd, colorEnd);
            Line line = Line(startpx, endpx);
            lines.push_back(line);
        }

        addPlane(MoveablePlane(offsetX, offsetY, lines, planeColor, priority));
    }
    inFile.close();
}

void Object::addPlane(MoveablePlane plane){
    this->planes.emplace_back(plane);
}


void Object::setPlanesColor(int color){
    for (MoveablePlane plane : this->getRefPlanes()){
        plane.setColor(color);
    }
}

void Object::setPos(Point position)
{
    this->position = position;
}

void Object::setPos(float x, float y)
{
    this->position = Point(x, y);
}

vector<MoveablePlane> Object::getPlanes() const
{
    return this->planes;
}

Point Object::getPos() const
{
    return this->position;
}

int Object::getWidth() const
{
    return this->xMax - this->xMin + 1;
}
int Object::getHeight() const
{
    return this->yMax - this->yMin + 1;
}

string Object::getId() const {
    return this->id;
}

vector<MoveablePlane> &Object::getRefPlanes(){
    return this->planes;
}
void Object::reverseHorizontal()
{
    // implement reverse as an object
    for (Plane &plane : planes)
    {
        for (Line &line : plane.getRefLines())
        {
            line.setStartPixel(Pixel(getWidth() - line.getStartPixel().getX() - 1, line.getStartPixel().getY(), line.getStartPixel().getColor()));
            line.setEndPixel(Pixel(getWidth() - line.getEndPixel().getX() - 1, line.getEndPixel().getY(), line.getEndPixel().getColor()));
        }
    }
}

bool Object::outOfWindow(int height, int width) const
{
    return position.getX() + xMax < 0 || position.getY() + yMax < 0 || position.getX() >= width || position.getY() >= height;
}

const vector<MoveablePlane> &Object::getConstRefPlanes() const {
    return this->planes;
}

void Object::calculate() {
    if(planes.empty()) return;

    xMin = planes[0].getRefPos().getX();
    yMin = planes[0].getRefPos().getY();
    xMax = planes[0].getRefPos().getX() + planes[0].getLowerRight().getX();
    yMax = planes[0].getRefPos().getY() + planes[0].getLowerRight().getY();

    for(int i=1;i<planes.size();++i){
        xMin = min(xMin, planes[i].getRefPos().getX());
        yMin = min(yMin, planes[i].getRefPos().getY());
        xMax = max(xMax, planes[i].getRefPos().getX() + planes[0].getLowerRight().getX());
        yMax = max(yMax, planes[i].getRefPos().getY() + planes[0].getLowerRight().getY());
    }

    xMax -= xMin;
    yMax -= yMin;

    for(MoveablePlane &plane : planes){
        plane.setPos(plane.getRefPos().getX() - xMin, plane.getRefPos().getY() - yMin);
    }

    position.setPoint(position.getX() + xMin, position.getY() + yMin);

    xMin = yMin = 0;
}

Point Object::getUpperLeft() const {
    return Point(xMin, yMin);
}

Point Object::getLowerRight() const {
    return Point(xMax, yMax);
}

void Object::sortPriority() {
    stable_sort(planes.begin(), planes.end(), [this](const MoveablePlane &a, const MoveablePlane &b) -> bool{
        return a.getPriority() < b.getPriority();
    });
}

Point &Object::getRefPos() {
    return position;
}

const Point &Object::getConstRefPos() const {
    return position;
}

void Object::writeToFile(ofstream &filePosition, ofstream &fileDetail) {
    filePosition << this->position.getX() << " ";
    filePosition << this->position.getY() << "\n";

    fileDetail << this->planes.size() << "\n";
    for (MoveablePlane& plane : this->planes) {
        plane.writeToFile(fileDetail);
    }
}
