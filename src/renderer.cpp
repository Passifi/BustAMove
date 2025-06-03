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

Graphic::Graphic(Transform2D* transform): transform(transform){}; 
Graphic::Graphic() : transform(nullptr) {};

void Graphic::updatePosition(Vector2D &newPosition) {
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
    
    rTarget->DrawEllipse(this->ellipse,this->brush,1.0f);
    if(this->filled) 
    rTarget->FillEllipse(this->ellipse,this->brush);
}

Circle::Circle(Transform2D* transform) : Graphic(transform) {
        ellipse = D2D1::Ellipse(Point2F(0.f,0.f), transform->dimension.x ,transform->dimension.y);
};

Arrow::Arrow(Transform2D* transform) : Graphic(transform) {

};


  
void Arrow::draw(ID2D1HwndRenderTarget* rTarget) {
   
    rTarget->DrawGeometry(shape,  brush);
    
    // update shape 

}

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
                
                Graphic* g = new Circle(properties.transPtr);
                g->brush = brush;
                return g;
            }
        case Arrow:
            {
                ID2D1SolidColorBrush *brush;
                ID2D1PathGeometry *path;
                HRESULT hr = rTarget->CreateSolidColorBrush(properties.color,&brush); 
                hr = factory->CreatePathGeometry(&path); 
                
                if(SUCCEEDED(hr)) {
                    
                    ID2D1GeometrySink *pSink = nullptr;
                    hr = path->Open(&pSink);
                        pSink->BeginFigure(D2D1::Point2F(20.f,0),D2D1_FIGURE_BEGIN_FILLED);
                        pSink->AddLine({40.f,50.f});
                        pSink->AddLine({25.f,50.f});
                        pSink->AddLine({25.f,120.f});
                        pSink->AddLine({15.f,120.f});
                        pSink->AddLine({15.f,50.f});
                        pSink->AddLine({0.f,50.f});
                        pSink->AddLine({20.f,0.f});
                         pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
                        hr = pSink->Close();
                    if(pSink != nullptr)
                         free(pSink);
                     
                    class::Arrow * g = new class::Arrow(properties.transPtr);
                    g->brush = brush; 
                    g->shape = path; 
                    return g; 
                }
            }
        default:
            return nullptr;
    }
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
   D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,&factory);
    createGraphicsResources();
}


ID2D1HwndRenderTarget* Renderer::getRenderTarget() {

    return this->rTarget;
}

ID2D1Factory * Renderer::getFactory() {
    return this->factory;
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
        applyMatrices(el->transform);
        el->draw(rTarget);
        rTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    }
    rTarget->EndDraw();
}

void Renderer::applyMatrices(Transform2D* transform) {

    this->rTarget->SetTransform(transform->getTransform());
};

void Renderer::addRenderObject(Graphic* graphic) {
     
    graphics.push_back(graphic);
}
