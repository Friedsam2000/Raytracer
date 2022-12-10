//
// Created by samuel on 20.11.22.
//

#ifndef RAYTRACER_GRID_H
#define RAYTRACER_GRID_H

#include <iostream>
#include <vector>
#include "Ray.h"
#include "Screen.h"
#include "Camera.h"
#include "../Dependencies/SDL2/include/SDL.h"
#include <algorithm>
#include "../Dependencies/SDL2/include/SDL_ttf.h"
#include "Coordinate.h"
#include <filesystem>
namespace fs = std::filesystem;




class Grid {

public:

    Grid(int width, int height){
        this->pixels = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
        this->width = width;
        this->height = height;
    }

    int width;
    int height;

    //A matrix of all the pixels in the grid
    std::vector<std::vector<int>> pixels;

    //A vector of all the screens in the grid
    std::vector<Screen> screens;

    //A vector of all the cameras in the grid (should only be one)
    Camera camera;

    //set pixel to Object
    bool setObject(Coordinate coordinate);

    //set pixel to Camera
    bool setCamera(Coordinate coordinate);

    //set pixel to Screen
    bool setScreen(Coordinate coordinate);

    //set a screen from x_start, y_start to x_end, y_end
    bool setScreen(Coordinate start, Coordinate end);

    //set an object from x_start, y_start to x_end, y_end
    bool setObject(Coordinate start, Coordinate end);

    //clear all Screens
    void clearScreens();

    //compute all rays in the grid (determined by camera and screen pairs), update the pixels and the rays vector
    void updateRays();

    //draw the grid in the console
    void drawInConsole();

    //check if a pixel is settable (free and in the grid)
    bool checkPixelSettable(Coordinate coordinate);

    //set screen with orientation (relative to x axis) and distance relative to a point (e.g camera)
    bool setScreenWithOrientationAndDistanceRelativeToPoint(Coordinate referencePoint, int distance_to_ref, float angle_relative_to_x_axis, int screen_length);

    //get the angle between two points, relative to the x axis
    static float getAngleBetweenTwoPoints(Coordinate point1, Coordinate point2);

    //handle SDL events
    void handleSDLEvent(SDL_Renderer *renderer, int grid_cell_size, SDL_Event sdl_event);

    //draw the grid in the SDL window
    void drawPixelsInSDL(SDL_Renderer *renderer, int grid_cell_size);

    //draw screens output in the SDL window
    void drawScreensInSDL(SDL_Renderer *renderer, int grid_cell_size);

    //overlay text in sdl window
    void overlayTextInSDL(SDL_Renderer *renderer, int grid_cell_size);

};



#endif //RAYTRACER_GRID_H
