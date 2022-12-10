//
// Created by samuel on 20.11.22.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include <iostream>
#include <vector>
#include <cmath>
#include "Coordinate.h"


class Ray {

public:

    Ray(Coordinate cameraPosition, Coordinate screenPosition){
        this->cameraPosition = cameraPosition;
        this->screenPosition = screenPosition;
    }

    //the ray is defined by these 4 properties
    Coordinate cameraPosition;
    Coordinate screenPosition;

    //return a vector of coordinates of the ray, takes a 2d vector of pixels as input
    std::vector<Coordinate> computeCoordinates(std::vector<std::vector<int>> &pixels) const;

    //return the length of the ray
    double computeLength(std::vector<std::vector<int>> &pixels);

    //Propagate a straight line in the pixel grid from a point in a direction
    //Stop (return -1, -1) if the line hits an object or end of screen
    //Otherwise return the coordinates of the next pixel in the line
    //Should be called in a loop until it returns -1, -1
    static Coordinate propagateLine(Coordinate currentPosition, Coordinate origin, Coordinate direction, std::vector<std::vector<int>> &pixels);

    };


#endif //RAYTRACER_RAY_H
