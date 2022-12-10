//
// Created by samuel on 20.11.22.
//

#include "Ray.h"


std::vector<Coordinate> Ray::computeCoordinates(std::vector<std::vector<int>> &pixels) const {

    //create a vector of coordinates for the ray and add camera position to it
    std::vector<Coordinate> rayCoordinates = {cameraPosition};

    while(true) {

        //propagate ray
        Coordinate currentRayCoordinate = propagateLine(rayCoordinates.back(), cameraPosition, screenPosition, pixels);

        //get width and height of the pixel grid
        int height = pixels[0].size();
        int width = pixels.size();

        //if the ray hits the edge of the grid, stop
        if (currentRayCoordinate.x == -1 || currentRayCoordinate.x == width || currentRayCoordinate.y == -1 || currentRayCoordinate.y == height ){
            break;
        }

        //if the ray hits an object, stop
        if (pixels[currentRayCoordinate.x][currentRayCoordinate.y] == 3){
            break;
        }

        //add the current ray coordinate to the vector of ray coordinates
        rayCoordinates.push_back(currentRayCoordinate);
    }

    return rayCoordinates;

}

double Ray::computeLength(std::vector<std::vector<int>> &pixels){

        //get the coordinates of the ray
        std::vector<Coordinate> coordinates = computeCoordinates(pixels);

        //initialize length
        double length = 0;


        //get start and end coordinates
        Coordinate start = coordinates.front();
        Coordinate end = coordinates.back();

        //compute length
        length = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));

        return length;
}

Coordinate Ray::propagateLine(Coordinate currentPosition, Coordinate origin, Coordinate direction, std::vector<std::vector<int>> &pixels){

    //check if ray goes right or left
    bool ray_goes_right = false;
    if(direction.y > origin.y) {
        ray_goes_right = true;
    }
    bool ray_goes_left = false;
    if(direction.y < origin.y) {
        ray_goes_left = true;
    }
    bool ray_goes_straight_down = false;
    if(direction.y == origin.y && direction.x > origin.x) {
        ray_goes_straight_down = true;
    }
    bool ray_goes_straight_up = false;
    if(direction.y == origin.y && direction.x < origin.x) {
        ray_goes_straight_up = true;
    }

    if(ray_goes_right) {
        float x_schnitt = ((currentPosition.y + 0.5 - origin.y) / (direction.y - origin.y)) * (direction.x - origin.x) + origin.x;

        //ray goes down (+x-direction)
        if (x_schnitt > currentPosition.x + 0.5) currentPosition.x++;
            //ray goes up (-x-direction)
        else if (x_schnitt < currentPosition.x - 0.5) currentPosition.x--;
            //ray goes right (+y-direction)
        else currentPosition.y++;
    }
    if(ray_goes_left) {
        float xschnitt = ((currentPosition.y - 0.5 - origin.y) / (direction.y - origin.y)) * (direction.x - origin.x) + origin.x;

        //ray goes down (+x-direction)
        if (xschnitt > currentPosition.x + 0.5) currentPosition.x++;
            //ray goes up (-x-direction)
        else if (xschnitt < currentPosition.x - 0.5) currentPosition.x--;
            //ray goes left (-y-direction)
        else currentPosition.y--;
    }
    if(ray_goes_straight_up) {
        currentPosition.x--;
    }
    if(ray_goes_straight_down) {
        currentPosition.x++;
    }
    
    
    //return the current ray coordinate
    return Coordinate(currentPosition.x, currentPosition.y);


}