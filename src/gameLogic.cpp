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


CircleCollisionShape::CircleCollisionShape() : CollisionShape() {
    this->name = "circle";

};


GameObject::GameObject(Vector2D pos) {
    this->transform->position =  pos;
}

bool RectCollisionShape::intersects(CollisionShape& other) {
        
    bool result = false;
   return result;
}

bool CircleCollisionShape::intersects(CollisionShape& other) {
    auto dist = this->transform->position - other.transform->position;
    return this->radius+20.0f >= dist.magnitude();
}

bool Collider::collision(Collider& collider) {
    
    bool collided = collider.shape->intersects(*this->shape); 
    if(collided) {
        collider.setCollisionStatus(true);
    }
    this->_hasCollided = this->_hasCollided || collided;
    //should there be a list of objects this collided with? 
    return collided;

}

void Collider::setCollisionStatus(bool newStat) {
this->_hasCollided = newStat; 
}


bool Collider::getCollisionStatus() {
    return this->_hasCollided;
}

void Collisionhandler::addCollider(Collider* collider) {
    this->colliders.push_back(collider);
}

void Collisionhandler::checkCollisions() {
// all collided objects are now marked, next 
    for(auto it = this->colliders.begin(); it != this->colliders.end(); it++) {
        for(auto cmp = it+1; cmp!= this->colliders.end(); cmp++)
        {
            if((*it)->collision(**cmp)) {
                
            }
        }

    };
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
     obj.collider->shape = new CircleCollisionShape(); 
     
    obj.collider->shape->transform = obj.transform; 
    
    this->gameObjects.push_back(obj);
    // alerta! must be fixed because this position is bound to change when resizing the vector! 
    return &this->gameObjects.back();
}


GameObject* GameHandler::addArrow() {

    GameObject* obj = new GameObject();
    obj->transform = new Transform2D({600.0f,400.0f},{1.0f,1.0f});
    obj->transform->rotationPoint = {25.f,120.f};  
    GraphicProperties properties {
        0,0, 
        100.0,
        100.0,
        1.0,
        false,
        D2D1::ColorF(0.9,0.2,0.9),
        obj->transform
    };
    obj->graphic = factory->createGraphic( Arrow, properties); 
    this->mainRender->addRenderObject(obj->graphic); 
    this->gameObjects.push_back(*obj);
    return obj;
}

void GameHandler::updateObjects() {
     
    for(auto& el : gameObjects) {
        
        if(el.collider->getCollisionStatus()) {
            el.velocity.x = 0.0f;
            el.velocity.y = 0.0f;
        }
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
