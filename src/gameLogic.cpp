#include "gamelogic.h"
#include "renderer.h"
#include <d2d1helper.h>
void GameObject::move(Vector2D offset) {
    this->transform->position += offset;    
    }

CollisionShape::CollisionShape()  { };
RectCollisionShape::RectCollisionShape() : CollisionShape() {
    this->name  = "rect"; 
    std::cout << "Created RectCollision" << std::endl;
};


GameObject::GameObject(Vector2D pos) {
    this->transform->position =  pos;
}

bool RectCollisionShape::intersects(CollisionShape& other) {
        
    bool result = false;
   return result;
}

bool Collider::collision(Collider& collider) {
    return collider.shape->intersects(*this->shape); 
}
GameObject* GameHandler::addGameobjectAt(Vector2D position) {
    GameObject obj;
    obj.transform = new Transform2D(position,{20.f,20.f});
    obj.collider->shape = new RectCollisionShape();
    GraphicProperties properties {
        0,0, 
        100.0,
        100.0,
        1.0,
        false,
        D2D1::ColorF(0.3,0.6,0.9),
        obj.transform
    };
    obj.graphic = factory->createGraphic(SphereGrapic, properties);
    this->mainRender->addRenderObject(obj.graphic); 
    this->gameObjects.push_back(obj);
    return &this->gameObjects.back();
}


void GameHandler::addArrow() {

    GameObject obj;
    obj.transform = new Transform2D({600.0f,400.0f},{1.0f,1.0f});
    obj.transform->rotationPoint = {25.f,120.f};  
    GraphicProperties properties {
        0,0, 
        100.0,
        100.0,
        1.0,
        false,
        D2D1::ColorF(0.9,0.2,0.9),
        obj.transform
    };
    obj.graphic = factory->createGraphic( Arrow, properties); 
     this->mainRender->addRenderObject(obj.graphic); 
    this->gameObjects.push_back(obj);
}

void GameHandler::updateObjects() {
    
    for(auto& el : gameObjects) {
        el.applyVelocity();
    }

}

void GameHandler::addGameobject(/*blueprint here*/) {
    GameObject obj;
    obj.transform = new Transform2D({100.f,100.f},{20.f,20.f});  
    obj.collider->shape = new RectCollisionShape();
    GraphicProperties properties {
        0,
        0,
        100.0,
        100.0,
        1.0,
        false,
        D2D1::ColorF(0.3,0.6,0.9),
        obj.transform
    };
    try { 
    obj.graphic = factory->createGraphic(SphereGrapic, properties);
    this->mainRender->addRenderObject(obj.graphic); 
    this->gameObjects.push_back(obj);
    }
    catch(int errorNum) {
        std::cout << errorNum << std::endl;
    }
    };


void GameObject::applyVelocity() {
    this->transform->setPosition(transform->position + this->velocity); // add a scalar to this for framedelta adjusted movement
}

GameHandler::GameHandler() {
    

};


GameObject::GameObject() {};
