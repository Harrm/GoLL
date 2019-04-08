cmake_minimum_required(VERSION 3.13)
project(FaceOfLife)

set(CMAKE_CXX_STANDARD 17)

add_executable(FaceOfLife main.cpp logic.h logic.cpp)
target_link_libraries(FaceOfLife sfml-graphics sfml-window sfml-system)
