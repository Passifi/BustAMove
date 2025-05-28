#include "gamelogic.h"
#include "renderer.h"
#include <d2d1helper.h>
void GameObject::move(Vector2D offset) {
        position += offset;
        graphic->updatePosition(position); 
    }

CollisionShape::CollisionShape() : x(0.0f),y(0.0f) { };
RectCollisionShape::RectCollisionShape() : CollisionShape(), width(10.0f), height(10.0f) {
    this->name  = "rect"; 
    std::cout << "Created RectCollision" << std::endl;
};


GameObject::GameObject(Vector2D pos) {
    this->position = pos;
}

bool RectCollisionShape::intersects(CollisionShape& other) {
        
    bool result = false;
    if(other.name == "rect") {
    auto castOther = static_cast<RectCollisionShape&>(other);
    result = this->x+this->width >= castOther.x &&
            this->x <= castOther.x + castOther.width &&
            this->y+this->height >= castOther.y &&
            this->y <= castOther.y + castOther.height;
    }
    return result;
}

bool Collider::collision(Collider& collider) {
    return collider.shape->intersects(*this->shape); 
}
void GameHandler::addGameobjectAt(Vector2D position) {
    GameObject obj(position);
     
    obj.collider->shape = new RectCollisionShape();
    GraphicProperties properties {
        obj.position.x,
        obj.position.y,
        100.0,
        100.0,
        1.0,
        false,
        D2D1::ColorF(0.3,0.6,0.9)

    };
    obj.graphic = factory->createGraphic(SphereGrapic, properties);
    this->mainRender->addRenderObject(obj.graphic); 
    this->gameObjects.push_back(obj);

}

void GameHandler::addGameobject(/*blueprint here*/) {
    GameObject obj;
     
    obj.collider->shape = new RectCollisionShape();
    GraphicProperties properties {
        obj.position.x,
        obj.position.y,
        100.0,
        100.0,
        1.0,
        false,
        D2D1::ColorF(0.3,0.6,0.9)

    };
    obj.graphic = factory->createGraphic(SphereGrapic, properties);
    this->mainRender->addRenderObject(obj.graphic); 
    this->gameObjects.push_back(obj);
    std::cout << "added gameobject" << std::endl;
};

GameHandler::GameHandler() {
    

};

GameObject::GameObject() {};
