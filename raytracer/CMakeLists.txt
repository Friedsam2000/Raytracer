cmake_minimum_required(VERSION 3.7)

project(raytracer)


set(CMAKE_CXX_STANDARD 17)

add_executable(raytracer src/main.cpp
        src/Camera.h
        src/Grid.cpp
        src/Grid.h
        src/Ray.cpp
        src/Ray.h
        src/Screen.h
        src/Coordinate.h)


target_include_directories(raytracer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Dependencies/SDL2/include)
target_link_libraries(raytracer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Dependencies/SDL2/lib/libSDL2-2.0.so.0.2600.1)
target_link_libraries(raytracer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Dependencies/SDL2/lib/libSDL2_ttf-2.0.so.0.14.1)

