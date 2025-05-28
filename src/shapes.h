#pragma once
#include <cstdint>
#include <windows.h>
#include <stdint.h>
#include <cmath>
#include <iostream>
#include "renderer.h"
#include "gameMath.h"
class Color {
    public:
    uint8_t red,green,blue,alpha;
    Color() : red(255),green(255),blue(255) {}; 
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha=255) : red(r), green(g), blue(b), alpha(alpha) {}
    Color(int refColor) {
        
        red = (refColor & 0xff000000) >> 24;
        green = (refColor & 0xff0000) >> 16;
        blue = (refColor & 0xff00) >> 8;
        alpha = (refColor & 0xff);

    }
private:
};
class Bubble {
    public:
    
    Bubble(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
    {
        color = 0xff00ff00;


    }
    float x,y,width,height;
        
    uint32_t color;
    Vector2D trajectory{0.0,0.0};  
    
    void setTrajectory(float x, float y);
    void move(); 
    bool collide(Bubble& other); 
    void setCirclePtr(Circle* ptr); 
    void setPosition(float x, float y);
    Circle* circle = nullptr;     
    private:


};

