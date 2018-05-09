cmake_minimum_required(VERSION 3.9)
project(Paint)

set(CMAKE_CXX_STANDARD 11)

add_executable(Paint
        main.cpp
        menu.h
        menu.rc FILL.h LINE.h UTIL.h CIRCLE.h CLIPPER.h LOGS.h)
