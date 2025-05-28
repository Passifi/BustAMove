#pragma once 
#include <d2d1.h>
#include <d2d1helper.h>
#include <windows.h>
#include <vector> 
#include <winefs.h>
#include <winerror.h>
#include <wingdi.h>
#include <winuser.h>
#include "renderer.h"
#include <iostream>
// define constants here 
//
//

Renderer::Renderer(HWND hwnd) : wHwnd(hwnd) {
            GetClientRect(hwnd,  &windowRect);
            calculateDisplayinfo(dispInfo,windowRect,20);
        }
Renderer::~Renderer() {
    std::cout << "Cleanup" << std::endl; 
    if(factory != nullptr) 
        factory->Release();
    
    if(rTarget != nullptr) 
        rTarget->Release();
    for(auto& el : graphics) {
        delete el;
    }
}


void initGraphicProperties(GraphicProperties& properties) {
    
    properties.x = 0;
    properties.y = 0;
    properties.width = 0;
    properties.height = 0;
    properties.lineWidth = 1.0f;
    properties.changed = false;
    properties.color = D2D1::ColorF(1.0,1.0,1.0);
}

Graphic::Graphic(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}; 
Graphic::Graphic() : x(0), y(0), width(0), height(0) {};

void Graphic::updatePosition(Vector2D &newPosition) {
    this->x = newPosition.x;
    this->y = newPosition.y;
    this->updateShape = true;    
}

void calculateDisplayinfo(DisplayInfo& info, RECT windowRect, int playfieldWidth) {
    const static float radius = 40.0f; 
    int height,width;
    width = windowRect.right - windowRect.left;
    height = windowRect.bottom - windowRect.top;
    info.displayHeight = height;
    info.displayWidth = width;
    info.middle = width >> 1;
    info.playfieldLeft = info.middle - (playfieldWidth*radius/2.0f);
    info.playfieldRight = info.middle + (playfieldWidth*radius/2.0)+radius*0.5f;
}

void Circle::draw(ID2D1HwndRenderTarget* rTarget)  {
    if(updateShape) { 
    this->ellipse = D2D1::Ellipse(Point2F(this->x,this->y),this->width  ,  this->height); 
    updateShape = false; }
    rTarget->DrawEllipse(this->ellipse,this->brush,1.0f);
    if(this->filled) 
    rTarget->FillEllipse(this->ellipse,this->brush);
}

Circle::Circle(float x, float y, float width, float height) : Graphic(x,y,width,height) {
        ellipse = D2D1::Ellipse(Point2F(x,y), width,height);
}; 
  

Graphic* GraphicsFactory::createGraphic(GraphicType type,const GraphicProperties& properties) {
    if(rTarget == nullptr) {
        return nullptr;
    }
   switch(type) {
    
        case SphereGrapic:
            {
               ID2D1SolidColorBrush *brush;
                    HRESULT hr = rTarget->CreateSolidColorBrush(properties.color,&brush); 
                    if(!SUCCEEDED(hr)) {
                    //maybe instead fallback on default brush, would depend on the type of error tho
                    return nullptr;
                }

                Graphic* g = new Circle(properties.x,properties.y,properties.width,properties.height);
                g->brush = brush;
                return g;
            }
        default:
            return nullptr;
    }
}

void Renderer::drawBoundaries(HDC hdc) {
}

void Renderer::drawGrid() {
    if(rTarget) {
        ID2D1SolidColorBrush *brush;
        rTarget->CreateSolidColorBrush(D2D1::ColorF(.0f,.0f,.0f),&brush);
        for(int i =0; i < dispInfo.displayWidth; i+=20) {
            rTarget->DrawLine(Point2F(i,0),Point2F(i,dispInfo.displayHeight)  , brush);
        }
        for(int i =0; i < dispInfo.displayHeight; i+=20) {
            rTarget->DrawLine(Point2F(0,i),Point2F(dispInfo.displayWidth,i)  , brush);
        }

         brush->Release();
    }
}

void Renderer::intialize() {
    std::cout << "Initializing Renderer" << std::endl;
   D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,&factory);
    createGraphicsResources();
}


ID2D1HwndRenderTarget* Renderer::getRenderTarget() {

    return this->rTarget;
}

HRESULT Renderer::createGraphicsResources() {
    
    HRESULT hr = S_OK;
    if(!rTarget) {
       RECT rc;
       GetClientRect(this->wHwnd,  &rc);
        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left, rc.bottom - rc.top
        );
       hr = factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
                                            D2D1::HwndRenderTargetProperties(wHwnd,size),
                                            &rTarget); 
    }
    // capture hr not being a success
    return hr;

};

void Renderer::updateDisplay() {
            createGraphicsResources(); // always resizes the client

        RECT rc;
        GetClientRect(this->wHwnd,  &rc);
       
        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left, rc.bottom - rc.top
        );
        rTarget->Resize(size); 

    // create new renderTarget or modify the old one? 
}
    
void Renderer::render() {
    HRESULT hr = createGraphicsResources();
    if(!SUCCEEDED(hr)) {
        return;
    }
    rTarget->BeginDraw();
    rTarget->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke)); 
    drawGrid();
    for(auto& el : graphics) {
        el->draw(rTarget);
    }
     
    rTarget->EndDraw();
}

void Renderer::addRenderObject(Graphic* graphic) {
     
    graphics.push_back(graphic);
}
