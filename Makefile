build:
	g++-7 -pthread main.cpp Master.cpp Line.cpp Plane.cpp MoveablePlane.cpp Object.cpp Pixel.cpp \
	MoveableObject.cpp PlaneFiller.cpp Point.cpp Rectangle.cpp -o main -lSDL2

test:
	g++-7 -pthread test.cpp Master.cpp Line.cpp Plane.cpp MoveablePlane.cpp Object.cpp Pixel.cpp \
	MoveableObject.cpp PlaneFiller.cpp Point.cpp Rectangle.cpp -o test -lSDL2

run:
	sudo ./main
