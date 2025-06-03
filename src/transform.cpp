#include "transform.h"

void Transform2D::rotatedAround(float degrees, Vector2D point) {
    this->rotationDegrees = degrees;
    this->rotationPoint = point;
    hasChanged = true; 
}
void Transform2D::setPosition(Vector2D newPosition) {
    this->position = newPosition;
    hasChanged = true;
}
void Transform2D::setScale(Vector2D newScale) {
    hasChanged = true;
}

void Transform2D::setRotation(float degrees) {
    
    this->rotationDegrees = degrees;
    hasChanged = true;

}

void Transform2D::updateTransform() {
    
    if(this->hasChanged) {

        this->rotationMat = rotationMatrix(this->rotationDegrees,this->rotationPoint+this->position);
        this->translationMat = translationMatrix(this->position);
        this->transformMat = this->translationMat*this->rotationMat;
        this->hasChanged = false;  
    }
}
 

D2D1_MATRIX_3X2_F Transform2D::getTransform() {
    
    updateTransform();
    return this->transformMat;
}

