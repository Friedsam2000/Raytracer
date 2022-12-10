//
// Created by samuel on 21.11.22.
//

#ifndef RAYTRACER_SCREEN_H
#define RAYTRACER_SCREEN_H

#include "Ray.h"
#include "Coordinate.h"

class Screen {

public:


    Screen(Coordinate position){
        this->position = position;
    }

    Coordinate position;

    //vector of all rays passing through screen
    std::vector<Ray> rays;




};


#endif //RAYTRACER_SCREEN_H
