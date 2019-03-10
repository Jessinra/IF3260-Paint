cmake_minimum_required (VERSION 2.6)
project (if3260_3d-model-plane-crasher)
add_executable(main main.cpp Line.cpp Master.cpp MoveableObject.cpp MoveablePlane.cpp Plane.cpp Pixel.cpp PlaneFiller.cpp Point.hpp Point.cpp Object.cpp Rectangle.cpp Rectangle.hpp)
add_executable(test test.cpp Line.cpp Master.cpp MoveableObject.cpp MoveablePlane.cpp Plane.cpp Pixel.cpp PlaneFiller.cpp Point.hpp Point.cpp Object.cpp Rectangle.cpp Rectangle.hpp)
