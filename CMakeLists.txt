cmake_minimum_required(VERSION 3.31)
project(SO2prj C)


set(CMAKE_C_STANDARD 23)

add_executable(SO2prj main.c
        header/functions.h
        header/structure.h
        header/costants.h
        source/structure.c
        source/functions.c
)
target_link_libraries(SO2prj m)
