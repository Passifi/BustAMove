#pragma once
#include "gameMath.h"
#include "d2d1.h"
#include <d2d1helper.h>
#define castPoint(a) D2D1::Point2F(a.x,a.y)       
inline  D2D1::Matrix3x2F translationMatrix(Vector2D a) 
{    
return D2D1::Matrix3x2F::Translation(a.x,a.y);
}
inline D2D1::Matrix3x2F rotationMatrix(float degrees,Vector2D point) {

return  D2D1::Matrix3x2F::Rotation(degrees,D2D1::Point2F( point.x,point.y));

}

class Transform2D {
    public: 
    Transform2D(Vector2D pos, Vector2D dim) : position(pos), dimension(dim),  rotationPoint(0.0f,0.0f){
       this->translationMat = translationMatrix(pos);
        this->rotationMat = rotationMatrix(rotationDegrees, Vector2D{0.0f,0.0f});
        transformMat = translationMat*rotationMat;   
    };
    Vector2D position;
    Vector2D dimension;
    float rotationDegrees = 0.0f;
    Vector2D rotationPoint;
    void rotatedAround(float degrees, Vector2D point); 
    void setRotation(float degrees); 
    void setPosition(Vector2D newPosition);
    void setScale(Vector2D newScale);
    void updateTransform(); 
    D2D1_MATRIX_3X2_F getTransform();
private:
    bool hasChanged = false;
    D2D1_MATRIX_3X2_F transformMat;
    D2D1_MATRIX_3X2_F translationMat;
    D2D1_MATRIX_3X2_F rotationMat;
    D2D1_MATRIX_3X2_F scalingMat;

};


