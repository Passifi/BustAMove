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
#include "transform.h"
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
        Graphic(Transform2D* transform); 
        Graphic();
        virtual  ~Graphic() = default; 
        virtual void draw(ID2D1HwndRenderTarget* rTarget) = 0;
         
        bool updateShape = false; 
        void updatePosition(Vector2D& newPosition); 
        void setColor(float red, float green, float blue); 
        void setColor(ID2D1Brush* brush);
        void release();
        ID2D1Brush *brush;        
        Transform2D* transform; 
        bool filled = true;
        bool toRemove = false;
};

class Circle :public Graphic {
    public:
        D2D1_ELLIPSE ellipse;
        Circle(Transform2D* transform);         
        void draw(ID2D1HwndRenderTarget* rTarget) override;
        
};

class Arrow: public Graphic {
    public:
        ID2D1PathGeometry *shape = nullptr;  
        Arrow(Transform2D* tranform);
        void draw(ID2D1HwndRenderTarget* rTarget) override;
};

struct GraphicProperties {
    
    float x,y, width, height,lineWidth;
    bool changed;
    D2D1::ColorF color;
    Transform2D* transPtr;
};

void initGraphicProperties();

enum GraphicType {
    BitmapGraphic,
    RectGraphic,
    SphereGrapic,
    Arrow,
};
class GraphicsFactory {
public:
    ID2D1HwndRenderTarget *rTarget = nullptr;
    ID2D1Factory *factory = nullptr; 
    Graphic* createGraphic(GraphicType type,const GraphicProperties& properties);
};

enum BrushId { 
    
    RedBrush = 0,
    GreenBrush = 1,
    BlueBrush = 2,
    PurpleBrush = 3,
    SilverBrush= 4,
    YellowBrush = 5,
    OrangerBrush = 6,

};



class Renderer {
    public:
        Renderer(HWND hwnd);
        ~Renderer();
        RECT windowRect;
        std::vector<ID2D1Brush*> brushes; 
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
        void addBrush(float red,float green,float blue);
        ID2D1Factory* getFactory(); 
        ID2D1HwndRenderTarget* getRenderTarget(); 
        void applyMatrices(Transform2D* transform);   
            
    private:
        HWND wHwnd;
        ID2D1Factory* factory = nullptr;
        ID2D1HwndRenderTarget* rTarget = nullptr;
};

