#include <memory>
#include <windows.h>
#include <wingdi.h>
#include <winnt.h>
#include <winuser.h>
#include <vector>
#include <random>
#include "shapes.h"
//#include "gamelogic.h"
#include <iostream>
#include "renderer.h"
#include "shapes.h"
#include "gameMath.h"
#include "input.h"
#include "gamelogic.h"
#include "transform.h"
#define PI 3.14159265358979323846
RECT windowRect;
const int playfieldWidth = 10;
const int playfieldMaxHeight = 30;
const float bubbleSpeed = 4.0f;
DisplayInfo dispInfo;
bool bubbleShot = false;
    bool running = true;
std::vector<std::uint32_t> colors {
    0xee0110,
    0x00ea00,
    0x0000a3,
    0xf0af00,
    0x333333,
    0xae00ee,
    0xe0ffa1,
    0x00ea0a,
};

GameHandler gameHandler;
Collisionhandler collisionHandler;
Renderer *render = nullptr;
Vector2D position(100.0f,100.0f);
GraphicsFactory graphicFactory;
GameObject *playerArrow = nullptr;
void setBubble() {
{
               int x = (int)(position.x / 20)*20;
               int y = (int)(position.y / 20)*20;
                   GraphicProperties props {
                    (float)x,
                    (float)y,
                    20,
                    20,
                    1.0,
                    false,
                    D2D1::ColorF(0.3f,0.8f,0.2f)
                };

                GameObject* g = gameHandler.addGameobjectAt( {(float)x,(float)y});         
                collisionHandler.addCollider(g->collider);         
    }
}

void shotBubble() {
                    float x = 600.0f;
                    float y = 400.0f;
                   GraphicProperties props {
                    (float)x,
                    (float)y,
                    20,
                    20,
                    1.0,
                    false,
                    D2D1::ColorF(0.3f,0.8f,0.2f)

                };
               auto ob=  gameHandler.addGameobjectAt( {(float)x,(float)y});         
                auto base = Vector2D(600.f,400.f); 
                auto res = position - base;
                res.normalize();
                ob->velocity= res*bubbleSpeed;
                collisionHandler.addCollider(ob->collider);
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int createWindow(HINSTANCE hInstance,LPSTR szAppName, int iCmdShow);
InputHandler inputs;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE , LPSTR lpCmdLine, int iCmdShow) {
    static TCHAR szAppName[] = TEXT ("HelloWin");
    int result = createWindow( hInstance, szAppName,  iCmdShow);
    if(result != 0) 
    {
        std::cout << "Couldn't intiliaze window, aborting" << std::endl;
    }
    MSG msg;
    inputs.registerAction("A", setBubble);
    while(running) { 
        while(PeekMessage(&msg, NULL,0,0,PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            }
            collisionHandler.checkCollisions();
            gameHandler.updateObjects();
            render->render();
    }
    delete render;
    return msg.wParam;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    switch(message) 
    {
            case WM_CREATE:
                GetClientRect(hwnd,  &windowRect);
                render = new Renderer(hwnd); 
                render->intialize();
                { 
                    graphicFactory.rTarget = render->getRenderTarget();
                    graphicFactory.factory = render->getFactory();
                    gameHandler.factory = &graphicFactory; 
                    gameHandler.mainRender = render;
                    gameHandler.addGameobject();
                    playerArrow = gameHandler.addArrow();  
                    // this doesn't work becuase when the vector gets resized the position of playerArrow changes 
                 
                }
                return 0;
            case WM_TIMER:
                return 0; 
                break; 
            case WM_PAINT:
                ValidateRect(hwnd, NULL ); 
                return 0;
            case WM_KEYDOWN:
                inputs.executeAction((int)wParam); 
                return 0; 
            case WM_MOUSEMOVE: {  
                    int kx = lParam & 0xffff;
                    int ky = (0xffff0000 & lParam) >> 16;
                    position = {(float)kx,(float)ky};
                    auto arrowBase= Vector2D{600.0f,400.0f};
                    auto degTarget = arrowBase - position;
                    playerArrow->transform->setRotation(-atan2(degTarget.x,degTarget.y)*(180/PI));
                    gameHandler.gameObjects[0].transform->setPosition( {(float)kx,(float)ky});
                }                
                return 0; 
            case WM_LBUTTONDOWN:
                if(!bubbleShot)  
                {
                    std::cout << gameHandler.gameObjects.size() << std::endl;
                shotBubble(); 
                bubbleShot = true; 

                    std::cout << gameHandler.gameObjects.size() << std::endl;
                } 
                return 0;
        case WM_LBUTTONUP:
                bubbleShot = false;
                return 0;
            case WM_DESTROY:
                running = false; 
                PostQuitMessage(0);
                return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
int createWindow(HINSTANCE hInstance,LPSTR szAppName, int iCmdShow) {
    HWND hwnd;
    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    if(!RegisterClass(&wndclass)) {
        MessageBox(NULL,TEXT("This program requirres Windows NT!"), szAppName, MB_ICONERROR);
        return 1;
    }
    hwnd = CreateWindow(szAppName, TEXT("The Hello Program"), WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInstance,NULL);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
    return 0;
}



