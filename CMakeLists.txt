cmake_minimum_required (VERSION 2.6)
project (if3260_3d-model-plane-crasher)
set(MAIN main.cpp)
set(TEST test.cpp)
set(LIBS Line.cpp Line.hpp Master.cpp Master.hpp MoveableObject.cpp MoveableObject.hpp MoveablePlane.cpp MoveablePlane.hpp
        Plane.cpp Plane.hpp Pixel.cpp Pixel.hpp PlaneFiller.cpp PlaneFiller.hpp Point.cpp Point.hpp
        Object.cpp Object.hpp Rectangle.cpp Rectangle.hpp MouseInputData.cpp MouseInputData.hpp View.cpp View.hpp)
set(CMAKE_CXX_FLAGS "-O2 -pthread -lSDL2 -lSDL2_image")
add_executable(main ${MAIN} ${LIBS})
add_executable(test ${TEST} ${LIBS})
add_executable(prev prev.cpp ${LIBS})
