//
// Created by samuel on 05.12.22.
//

#ifndef RAYTRACER_COORDINATE_H
#define RAYTRACER_COORDINATE_H

#include <math.h>


class Coordinate {

public:
    Coordinate(int x, int y){
        this->x = x;
        this->y = y;
    }
    Coordinate(){
        this->x = 0;
        this->y = 0;
    }

    Coordinate operator+ (const Coordinate & first) const
    {
        return Coordinate(x + first.x, y + first.y);
    }

    Coordinate operator- (const Coordinate & first) const
    {
        return Coordinate(x - first.x, y - first.y);
    }

    int x;
    int y;
};


#endif //RAYTRACER_COORDINATE_H
