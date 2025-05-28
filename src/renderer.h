#pragma once 
#include <wchar.h>
#include <windows.h>
#include <vector> 
#include <unordered_map>
#include <winefs.h>
#include <wingdi.h>
#include <winuser.h>
#include <iostream>
#include <d2d1.h>
#include <d2d1_1.h>
#include "gameMath.h"
// define constants here 
//
#define ToColorF(color) D2D1::ColorF(color.red/255,color.green/255,color.blue/255)  
struct DisplayInfo {
    int playfieldLeft;
    int playfieldRight;
    int middle;
    int displayHeight;
    int displayWidth;
};
using D2D1::Point2F ;


void calculateDisplayinfo(DisplayInfo& info, RECT windowRect, int playfieldWidth);

class Graphic {
    
    public:
        Graphic(float x, float y, float width, float height); 
        Graphic();
        virtual  ~Graphic() = default; 
        virtual void draw(ID2D1HwndRenderTarget* rTarget) = 0;
        bool updateShape = false; 
        void updatePosition(Vector2D& newPosition); 
        ID2D1Brush *brush;        
        float x,y,width,height;
        bool filled = true;
};

class Circle :public Graphic {
    public:
        D2D1_ELLIPSE ellipse;
        Circle(float x, float y, float width, float height);         
        void draw(ID2D1HwndRenderTarget* rTarget) override;
        void setColor(ID2D1Brush* brush);
        
}
;

struct GraphicProperties {
    
    float x,y, width, height,lineWidth;
    bool changed;
    D2D1::ColorF color;
};

void initGraphicProperties();

enum GraphicType {
    BitmapGraphic,
    RectGraphic,
    SphereGrapic,
};
class GraphicsFactory {
public:
    ID2D1HwndRenderTarget *rTarget = nullptr;
     
    Graphic* createGraphic(GraphicType type,const GraphicProperties& properties);
        
};

class Renderer {
    public:
        Renderer(HWND hwnd);
        ~Renderer();
        RECT windowRect;
        std::vector<Graphic*> graphics;
        PAINTSTRUCT ps; 
        DisplayInfo dispInfo; 
        HRESULT createGraphicsResources();
        void intialize();
        void drawBoundaries(HDC hdc);
        void render();
        void updateDisplay();
        void addRenderObject(Graphic* graphics);
        void removeObject();
        void drawGrid(); 
        
        ID2D1HwndRenderTarget* getRenderTarget(); 
    private:
        HWND wHwnd;
        ID2D1Factory* factory = nullptr;
        ID2D1HwndRenderTarget* rTarget = nullptr;
};

