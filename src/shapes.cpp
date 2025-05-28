#include "shapes.h"
void Bubble::setTrajectory(float x, float y) {

    trajectory = {x,y};
}
void Bubble::move() {
    this->x += this->trajectory.x;
    this->y += this->trajectory.y;

    if(circle != nullptr) {
        circle->x = x;
        circle->y = y;
    }
}

void Bubble::setPosition(float x, float y) {

    this->x = x;
    this-> y = y;
    this->circle->x = x;
    this->circle->y = y;

}

bool Bubble::collide(Bubble& other) {

    Vector2D center(this->x + this->width/2.0f,this->y + this->height/2.0f);
    Vector2D center2(other.x + other.width/2.0f,other.y + other.height/2.0f);
    Vector2D delta = center2 - center;
    if(delta.magnitude() <= (this->width/2.0f + other.width/2.0f)){
        this->trajectory.x = 0;
        this->trajectory.y = 0;
        return true; 
    }
    return false; 
};

void Bubble::setCirclePtr(Circle* ptr) {
    
    this->circle = ptr;

}




