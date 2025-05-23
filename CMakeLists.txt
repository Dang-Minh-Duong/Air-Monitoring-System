cmake_minimum_required(VERSION 3.16.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(new)

include_directories(${PROJECT_DIR}/include)

idf_component_register(
    SRCS "src/main.c" "src/pwm.c"
    INCLUDE_DIRS "include"
)
