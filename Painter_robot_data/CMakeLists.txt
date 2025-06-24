cmake_minimum_required(VERSION 2.8.3)
project(Painter_Robot)

set(CMAKE_CXX_FLAGS "-std=c++11 -O2 ${CMAKE_CXX_FLAGS}")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(paint_robot
    src/main.cpp
    src/robot.cpp
    src/opengl_display.cpp    
    src/path_finder.cpp
)

include_directories(EIGEN3_INCLUDE_DIR)
include_directories(/usr/local/include/eigen3)
include_directories(/usr/include/eigen3)

target_link_libraries(paint_robot ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
