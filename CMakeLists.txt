cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#project( resize )
add_executable(resize resize.cpp )
add_executable(sort sort.cpp )
add_executable( YUV4202RGB888 YUV4202RGB888.cpp )

#include_directories( ncnn/build/src )
#include_directories( ncnn/src )
#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/ncnn/include )

MESSAGE(${CMAKE_CURRENT_BINARY_DIR})

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

target_link_libraries( resize ${OpenCV_LIBS})
target_link_libraries( sort ${OpenCV_LIBS})
