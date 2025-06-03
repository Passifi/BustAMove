#pragma once
#include <iostream>
#include <cmath>

class Vector2D  {

public:
     float x, y;
     Vector2D(float x, float y) : x(x), y(y) {}
     Vector2D operator+(Vector2D& other) {
      return {this->x + other.x, this->y + other.y}; 
    }
     Vector2D operator-(Vector2D& other) {
        
        return {this->x - other.x, this->y - other.y};

    }
     Vector2D operator*(float constValue) {
        return {this->x*constValue, this->y*constValue};
    }
     Vector2D& operator+=(Vector2D& other) {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    bool operator==(Vector2D& other) {
        return this->x == other.x && this->y == other.y;
    }

     float magnitude() {
        return sqrt(this->x*this->x + this->y * this->y);
    }
     void normalize() {
        
        float mag = this->magnitude();
        this->x = this->x/mag;
        this->y = this->y/mag;

    }
private:
    

};



