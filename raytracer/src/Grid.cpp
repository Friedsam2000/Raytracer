#define DISTANCE_TO_SCREEN 40
#define SCREEN_WIDTH 20
#define SCREEN_SDL_WIDTH 6

#include "Grid.h"

bool Grid::setObject(Coordinate coordinate) {
    if(checkPixelSettable(coordinate)) {
        //set pixel to object
        pixels[coordinate.y][coordinate.x] = 3;
//        std::cout << "Object set at " << x << ", " << y << std::endl;
        return true;
    }else{
        return false;
    }

}

bool Grid::setCamera(Coordinate coordinate) {

    if(checkPixelSettable(coordinate)) {
        //set old camera pixel to empty
        pixels[camera.position.x][camera.position.y] = 0;
        //move camera to coordinate
        camera.position = coordinate;
        //set pixel to camera
        pixels[coordinate.x][coordinate.y] = 1;
        return true;
        }
    return false;

}

bool Grid::setScreen(Coordinate coordinate){

    if(checkPixelSettable(coordinate)){
        //set the pixel to screen
        pixels[coordinate.y][coordinate.x] = 3;
        //add a screen to screens vector
        Screen screen(coordinate);
        screens.push_back(screen);
        return true;
    }else{
        return false;
    }
}

bool Grid::setScreen(Coordinate start, Coordinate end) {

    //Check if screen start and end are free and in the grid
    if (not(checkPixelSettable(start) && checkPixelSettable(end))) {
        std::cout << "Screen start or end is not free" << std::endl;
        return false;
    }

    //create a vector of coordinates for the screen and add the start coordinate
    std::vector<Coordinate> screenCoordinates = {start};

    while(true) {

        //create a pointer to a vector of 2 ints
        Coordinate currentScreenCoordinate = Ray::propagateLine(screenCoordinates.back(), start, end, pixels);

        //if the screen hits another screen, stop
        if (pixels[currentScreenCoordinate.x][currentScreenCoordinate.y] == 2){
            std::cout << "Screen could not be set, another screen in the way!" << std::endl;
            return false;
        }

        //if the screen hits the edge of the grid, stop
        if(currentScreenCoordinate.x >= height || currentScreenCoordinate.y >= width || currentScreenCoordinate.x < 0 || currentScreenCoordinate.y < 0){
            std::cout << "Screen could not be set, end of grid reached!" << std::endl;
            return false;
        }

        //if the screen hits an object, stop
        if (pixels[currentScreenCoordinate.x][currentScreenCoordinate.y] == 3){
            std::cout << "Screen could not be set, object in the way!" << std::endl;
            return false;
        }

        //if the screen hits the end of the screen, stop
        if (currentScreenCoordinate.x == end.x && currentScreenCoordinate.y == end.y){
            break;
        }

        //add the current screen coordinate to the vector of screen coordinates
        screenCoordinates.push_back(currentScreenCoordinate);

    }

    //create a screen for each coordinate in the vector of screen coordinates
    for (Coordinate coordinate : screenCoordinates) {
        Screen screen(coordinate);
        screens.push_back(screen);
    }
    std::cout << "Screen set from " << start.x << ", " << start.y << " to " << end.x << ", " << end.y << std::endl;

    return true;
}

void Grid::clearScreens() {

    //clear all rays from all screens
    for(int i = 0; i < screens.size(); i++){
        //set all ray pixels to empty
        for(int j = 0; j < screens[i].rays.size(); j++){

            //compute the coordinates of the ray
            std::vector<Coordinate> rayCoordinates = screens[i].rays[j].computeCoordinates(pixels);

            //set all ray pixels to empty
            for(int k = 0; k < rayCoordinates.size(); k++){
                pixels[rayCoordinates[k].x][rayCoordinates[k].y] = 0;
            }
        }
        screens[i].rays.clear();
    }

    //set all screen pixels to empty
    for(int i = 0; i < screens.size(); i++){
        pixels[screens[i].position.x][screens[i].position.y] = 0;
    }
    //clear all screens
    screens.clear();

    std::cout << "Cleared all screens" << std::endl;
}

void Grid::drawInConsole(){
    for (int i = 0; i < pixels.size(); i++) {
        for (int j = 0; j < pixels[i].size(); j++) {
            if (pixels[i][j] == 0) {
                std::cout << "'";
            } else if (pixels[i][j] == 1) {
                std::cout << "C";
            } else if (pixels[i][j] == 2) {
                std::cout << "S";
            } else if (pixels[i][j] == 3) {
                std::cout << "X";
            } else if (pixels[i][j] == 4) {
                std::cout << "R";
            }
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void Grid::updateRays(){

    //clear all rays from all screens
    for(int i = 0; i < screens.size(); i++){
        //set all ray pixels to empty
        for(int j = 0; j < screens[i].rays.size(); j++){

            //compute the coordinates of the ray
            std::vector<Coordinate> ray_coordinates = screens[i].rays[j].computeCoordinates(pixels);

            //set all ray pixels to empty
            for(int k = 0; k < ray_coordinates.size(); k++){
                pixels[ray_coordinates[k].x][ray_coordinates[k].y] = 0;
            }
        }
        screens[i].rays.clear();
    }

    //create a ray for each camera and screen pair
    for (int j = 0; j < screens.size(); j++) {
        //Define a new ray
        Ray ray(camera.position, screens[j].position);
        //Add the ray to the screen
        screens[j].rays.push_back(ray);
    }


    //compute the coordinates of all the rays in all the screens
    for (int i = 0; i < screens.size(); i++) {
        for(int j = 0; j < screens[i].rays.size(); j++) {
            std::vector<Coordinate> rayCoordinates = screens[i].rays[j].computeCoordinates(pixels);
            //set the pixel of all the ray coordinates to ray
            for(int k = 0; k < rayCoordinates.size(); k++){
                pixels[rayCoordinates[k].x][rayCoordinates[k].y] = 4;
            }
        }
    }

    //overwrite ray pixels with camera and screen
    pixels[camera.position.x][camera.position.y] = 1;
    for (int i = 0; i < screens.size(); i++) {
        pixels[screens[i].position.x][screens[i].position.y] = 2;
    }
}

bool Grid::checkPixelSettable(Coordinate coordinate) {

    //check if pixel is on the grid
    if (coordinate.x >= height || coordinate.y >= width || coordinate.x < 0 || coordinate.y < 0) {
        std::cout << "Pixel is not on the grid" << std::endl;
        return false;
    }

    //check if pixel is free or ray
    if (pixels[coordinate.x][coordinate.y] == 0 || pixels[coordinate.x][coordinate.y] == 4) {
        return true;
    } else {
        std::cout << "Pixel is not free" << std::endl;
        return false;
    }
}

bool Grid::setScreenWithOrientationAndDistanceRelativeToPoint(Coordinate referencePoint, int distance_to_ref, float angle_relative_to_x_axis, int screen_length){

    //create a vector CM (from ref to middle point of screen) with 2 elements, x and y
    Coordinate CM;
    CM.x = distance_to_ref * cos(angle_relative_to_x_axis * M_PI / 180);
    CM.y = - distance_to_ref * sin(angle_relative_to_x_axis * M_PI / 180);

    //create a vector MS (from middle point of screen to screen start) with 2 elements, x and y
    Coordinate MS;
    MS.x = screen_length / 2 * cos((angle_relative_to_x_axis + 90) * M_PI / 180);
    MS.y = - screen_length / 2 * sin((angle_relative_to_x_axis + 90) * M_PI / 180);

    //create a vector ME (from middle point of screen to screen end) with 2 elements, x and y
    Coordinate ME;
    ME.x = screen_length / 2 * cos((angle_relative_to_x_axis - 90) * M_PI / 180);
    ME.y = - screen_length / 2 * sin((angle_relative_to_x_axis - 90) * M_PI / 180);

    //set screen
    if(setScreen(referencePoint + CM + MS, referencePoint + CM + ME)){
        return true;
    } else {
        return false;
    }

}

float Grid::getAngleBetweenTwoPoints(Coordinate point1, Coordinate point2){
    //compute the angle between the two points relative to the x axis
    float angle = 180-atan2(point2.y - point1.y, point2.x - point1.x) * 180 / M_PI;
    //return the angle
    return angle;
}

void Grid::handleSDLEvent(SDL_Renderer *renderer, int grid_cell_size, SDL_Event sdl_event) {

        //display current event if there is a new event
        if (sdl_event.type == SDL_KEYDOWN) {
            //if key is an arrow key
            if (sdl_event.key.keysym.sym == SDLK_UP || sdl_event.key.keysym.sym == SDLK_DOWN || sdl_event.key.keysym.sym == SDLK_LEFT || sdl_event.key.keysym.sym == SDLK_RIGHT) {
                switch(sdl_event.key.keysym.sym) {
                    case SDLK_UP:
                        //check if pixel is settable
                        if (checkPixelSettable(Coordinate(camera.position.x-1, camera.position.y))) {
                            //move camera up
                            setCamera(Coordinate(camera.position.x-1, camera.position.y));
                            }else {
                            //display error: Camera could not be moved
                            std::cout << "Error: Camera could not be moved" << std::endl;
                        }
                        break;
                    case SDLK_DOWN:
                        //check if pixel is settable
                        if (checkPixelSettable(Coordinate(camera.position.x+1, camera.position.y))) {
                            //move camera down
                            setCamera(Coordinate(camera.position.x+1, camera.position.y));
                        }else {
                            //display error: Camera could not be moved
                            std::cout << "Error: Camera could not be moved" << std::endl;
                        }
                        break;
                    case SDLK_LEFT:
                        //check if pixel is settable
                        if (checkPixelSettable(Coordinate(camera.position.x, camera.position.y-1))) {
                            //move camera left
                            setCamera(Coordinate(camera.position.x, camera.position.y-1));
                        }else {
                            //display error: Camera could not be moved
                            std::cout << "Error: Camera could not be moved" << std::endl;
                        }
                        break;
                    case SDLK_RIGHT:
                        //check if pixel is settable
                        if (checkPixelSettable(Coordinate(camera.position.x, camera.position.y+1))) {
                            //move camera right
                            setCamera(Coordinate(camera.position.x, camera.position.y+1));
                        }else {
                            //display error: Camera could not be moved
                            std::cout << "Error: Camera could not be moved" << std::endl;
                        }
                        break;

                }
            }
        }

        //if mouse button is pressed
        if (sdl_event.type == SDL_MOUSEBUTTONDOWN && sdl_event.button.button == SDL_BUTTON_LEFT) {
            //clear the screens
            clearScreens();
            //create a new screen with angle between camera and mouse
            setScreenWithOrientationAndDistanceRelativeToPoint(camera.position, DISTANCE_TO_SCREEN, getAngleBetweenTwoPoints(Coordinate(sdl_event.button.y / grid_cell_size, sdl_event.button.x / grid_cell_size), camera.position), SCREEN_WIDTH);
        }

    //compute all found rays, add them to the screen and update the pixels
    updateRays();
}


void Grid::drawPixelsInSDL(SDL_Renderer *renderer, int grid_cell_size){
    //set SDL pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            switch (pixels[i][j]) {
                case 0:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    break;
                case 1:
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    break;
                case 2:
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    break;
                case 3:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    break;
                case 4:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    break;
            }
            SDL_Rect rect;
            rect.x = j * grid_cell_size;
            rect.y = i * grid_cell_size;
            rect.w = grid_cell_size;
            rect.h = grid_cell_size;
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

bool Grid::setObject(Coordinate start, Coordinate end){

    //check if object start and end are on the grid
    if(not(checkPixelSettable(start) && checkPixelSettable(end))){
        //display error: Object could not be set
        std::cout << "Error: Object could not be set, start or end not on grid" << std::endl;
        return false;
    }

    //create a vector of coordinates for the object
    std::vector<Coordinate> objectCoordinates = {start};

    while(true) {

        //propagate ray, return -1 if ray hits object or end of grid
        Coordinate currentObjectCoordinate = Ray::propagateLine(objectCoordinates.back(), start,end, pixels);

        //if the object hits an object or the end of the grid, stop
        if (currentObjectCoordinate.x == -1){
            std::cout << "Object could not be set, another object or end of grid in the way!" << std::endl;
            return false;
        }

        //if the Object hits a screen, stop
        if (pixels[currentObjectCoordinate.x][currentObjectCoordinate.y] == 2){
            std::cout << "Object could not be set, screen in the way!" << std::endl;
            return false;
        }

        //if the object hits a camera, stop
        if (pixels[currentObjectCoordinate.x][currentObjectCoordinate.y] == 2){
            std::cout << "Object could not be set, camera in the way!" << std::endl;
            return false;
        }

        //add the current object coordinate to the vector of object coordinates
        objectCoordinates.push_back(currentObjectCoordinate);

        //if the object hits the end, stop
        if (currentObjectCoordinate.x == end.x && currentObjectCoordinate.y == end.y){
            break;
        }


    }

    //create an object for each coordinate in the vector of screen coordinates
    for (int i = 0; i < objectCoordinates.size(); i++) {
        pixels[objectCoordinates[i].x][objectCoordinates[i].y] = 1;
    }
    std::cout << "Object set from " << start.x << "," << start.y << " to " << end.x << "," << end.y << std::endl;
    return true;
}

void Grid::drawScreensInSDL(SDL_Renderer *renderer, int grid_cell_size){

    //check if there are any screens
    if (screens.size() == 0){
        return;
    }
    
    int size_of_one_screen = SCREEN_SDL_WIDTH * grid_cell_size;

    //create a vector of lengths of the rays in all screens
    std::vector<float> ray_lengths;

    //for all screens in the grid
    for (int i = 0; i < screens.size(); i++) {
        //get the length of the ray in the current screen
        ray_lengths.push_back(screens[i].rays[0].computeLength(pixels));
    }

    //get the max length of the rays
    float max_length = *std::max_element(ray_lengths.begin(), ray_lengths.end());

    //get the min length of the rays
    float min_length = *std::min_element(ray_lengths.begin(), ray_lengths.end());

    //set the maximum brightness value
    float max_brightness = 255;

    //reverse the order of the ray lengths
    std::reverse(ray_lengths.begin(), ray_lengths.end());

    //for every screen in the grid
    for (int i = 0; i < screens.size(); i++) {
        //set the color of the screen according to the normalized length of the ray, the shorter the ray, the brighter the screen
        float brightness = (ray_lengths[i] - max_length) / (min_length - max_length) * max_brightness;
        //differentiate the brightness more to make the difference more visible
        brightness = (brightness * brightness*brightness) / (max_brightness*max_brightness);


        SDL_SetRenderDrawColor(renderer, 0, 0, brightness, 255);
        //
        //
        // SDL_SetRenderDrawColor(renderer, 0, 0, brightness, 255);
        // create a rectangle for the screen
        SDL_Rect rect;
        //set the squares in the bottom right corner next to each other
        rect.x = (width ) * grid_cell_size - (i + 1) * size_of_one_screen;
        rect.y = (height ) * grid_cell_size - size_of_one_screen;

        //set the size of the rectangle
        rect.w = size_of_one_screen;
        rect.h = size_of_one_screen;
        //draw the rectangle
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Grid::overlayTextInSDL(SDL_Renderer *renderer, int grid_cell_size){

    //get the project path
    std::string path = SDL_GetBasePath();

    //go up one directory by checking for the second last slash
    int pos = path.find_last_of("/\\", path.length() - 2);
    path = path.substr(0, pos);

    //add the path to the font
    path += "/fonts/Aero.ttf";

    //print the path to the font
//    std::cout << path << std::endl;


    //init font
    TTF_Font *font;
//    std::cout << "trying to open font from path: "<< path << std::endl;
    font = TTF_OpenFont(path.c_str(), 24);
    if (font == NULL) {
        std::cout << "Error: Could not open font" << std::endl;
        return;
    }

    //set the color of the text to grey
    SDL_Color color = {255, 255, 255};


    //print the first line of text
    SDL_Surface *surfaceMessage = TTF_RenderText_Blended_Wrapped(font, "Move the camera by using the arrow keys.    "
                                                                       " Place the screen by clicking anywhere.      "
                                                                       " Observe the screen in the bottom right corner.", color,600);
    SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.x = width/1.5 * grid_cell_size;
    Message_rect.y = 30;
    Message_rect.w = width*grid_cell_size - width/1.5 * grid_cell_size-10;
    Message_rect.h = 100;
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);


    //free the memory from all lines
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
    TTF_CloseFont(font);
}

