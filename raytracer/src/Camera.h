//
// Created by samuel on 21.11.22.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Coordinate.h"

class Camera {

public:

    Camera(Coordinate postition){
        this->position = postition;
    }

    Camera(){
        this->position = Coordinate();
    }

    Coordinate position;


};


#endif //RAYTRACER_CAMERA_H
