#include "gamelogic.h"
#include "renderer.h"
#include <cmath>
#include <d2d1helper.h>
#include <memory>
#include <sys/stat.h>
#include <algorithm>
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

bool CircleCollisionShape::intersects([[maybe_unused]]CollisionShape& other) {
    auto dist = this->transform->position - other.transform->position;
    return this->radius+20.0f >= dist.magnitude();
}

void Collider::release() {
        this->toRemove = true;
}
void checkConnections(Bubble* root) {
    std::unordered_set<Bubble*> visited;
    std::queue<Bubble*> toVisit;
    toVisit.push(root); 
    for(auto&el : root->connections) {
        if(root->color == el->color) // only visit nodes which have the same colora 
        toVisit.push(el); 
    }
    
    while(toVisit.size() > 0) {
        auto current = toVisit.front();
        toVisit.pop();
        if(current != nullptr) {
            visited.insert(current); 
        }
        else 
            continue;
        for(auto&el : current->connections) {
            
            if(visited.count(el) > 0) 
                continue; // don't visit nodes twice

            if(current->color == el->color) 
                toVisit.push(el);
        }
    }
    if(visited.size() > 2) {
        for(auto& el : visited) {
            el->toRemove = true;
        }
    }
};

bool Collider::collision(Collider& collider) {
    
    bool collided = collider.shape->intersects(*this->shape); 
    if(collided) {
        collider.setCollisionStatus(true);
        this->collidedWith.push_back(&collider); 
    }
    this->_hasCollided = this->_hasCollided || collided;
    return collided;
}

void Collider::setCollisionStatus(bool newStat) {
    this->_hasCollided = newStat; 
}

bool Collider::getCollisionStatus() {
    return this->_hasCollided;
}

void Collider::resetCollisionStatus() {
    this->_hasCollided = false;
}

void Collisionhandler::addCollider(Collider* collider) {
    this->colliders.push_back(collider);
}

void Collisionhandler::checkCollisions() {
// all collided objects are now marked, next 
    for(auto it = this->colliders.begin(); it != this->colliders.end(); it++) {
        for(auto cmp = it+1; cmp!= this->colliders.end(); cmp++)
        {
            if(*it == nullptr || (*it)->toRemove) continue;
             if((*it)->collision(**cmp)) {
            }
        }

    };
}

void Bubble::connectTo(Bubble* other) {
    this->connections.push_back(other);
}

GameObject* GameHandler::addGameobjectAt(Vector2D position) {

    auto colorIndex = rand()%6; 
    std::unique_ptr<Bubble> obj = std::make_unique<Bubble>(colorIndex);
    obj->transform = new Transform2D(position,{20.f,20.f});
    obj->collider->shape = new RectCollisionShape();
    obj->collider->onCollision = []() { 
    };
    GraphicProperties properties {
        0,0, 
        100.0,
        100.0,
        1.0,
        false,
        D2D1::ColorF(0.3,0.6,0.9),
        obj->transform
    };
    obj->graphic = factory->createGraphic(SphereGrapic, properties);
    obj->graphic->brush = this->mainRender->brushes[colorIndex]; 
    this->mainRender->addRenderObject(obj->graphic); 
    obj->collider->shape = new CircleCollisionShape(); 
    obj->collider->shape->transform = obj->transform; 
    this->gameObjects.push_back(std::move(obj));
    return this->gameObjects.back().get();
}



Pointer* GameHandler::addArrow(Vector2D position) {
    std::unique_ptr<Pointer> obj = std::make_unique<Pointer>(position);
    obj->transform = new Transform2D({position.x,position.y-120.0f},{1.0f,1.0f});
    obj->transform->rotationPoint = {0.f,120.f};  
    obj->collider->onCollision = []() {std::cout << "Collided" << std::endl;};
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
    this->gameObjects.push_back(std::move(obj));
    return static_cast<Pointer*>(this->gameObjects.back().get());

}

GameObject* GameHandler::getObjectFromCollider(Collider* c) {

    for(auto& g:this->gameObjects) {
        if(g->collider == c) 
            return g.get();
    }
    return nullptr;
}

void GameHandler::updateObjects() {
    collisionHandler->checkCollisions(); 
    
    for(auto& el : gameObjects) {
        
        if(el->collider->getCollisionStatus()) {
            if(el->type == "Bubble") {
            for(auto& c : el->collider->collidedWith) {
                 auto connectTo = this->getObjectFromCollider(c);
                if(connectTo != nullptr && connectTo->type == "Bubble") 
                {
                        auto b = static_cast<Bubble*>(el.get());
                        b->connectTo(static_cast<Bubble*>(connectTo)); 
                        
                }
            }
            }
            static float x,y; 
            if(el->velocity.magnitude()  > 0) {
                el->graphic->brush = mainRender->brushes.front();
                x = el->transform->position.x;
                y = el->transform->position.y;
                x = floor(x/20.0f)*20.0f;   
                y = (floor(y/20.0f)+1)*20.0f;   
                el->transform->setPosition({x,y});
                el->velocity.x = 0.0f;
                el->velocity.y = 0.0f;
            }
            el->collider->resetCollisionStatus();
        }
        el->applyVelocity();
        if(el->type == "Bubble") {
            
            auto cBubble = static_cast<Bubble*>(el.get());
            checkConnections(cBubble);  
        }
    }
    auto new_end = std::remove_if(gameObjects.begin(),gameObjects.end(),
                                  [](const std::unique_ptr<GameObject>& b_ptr) {
                                        
                                    return b_ptr && b_ptr->toRemove;

                                  });
    gameObjects.erase(new_end,gameObjects.end());
}

void collisionFunction() {
    std::cout << "Collided" << std::endl;
}

void GameHandler::addGameobject(/*blueprint here*/) {
    std::unique_ptr<GameObject> obj = std::make_unique<GameObject>();
    obj->transform = new Transform2D({100.f,100.f},{20.f,20.f});  
    obj->collider->shape = new RectCollisionShape();
    obj->collider->onCollision = []() {};
    GraphicProperties properties {
        0,
        0,
        100.0,
        100.0,
        1.0,
        false,
        D2D1::ColorF(0.3,0.6,0.9),
        obj->transform
    };
    try { 
    obj->graphic = factory->createGraphic(SphereGrapic, properties);
    obj->graphic->brush = mainRender->brushes[OrangerBrush]; 
    this->mainRender->addRenderObject(obj->graphic); 
    this->gameObjects.push_back(std::move(obj));
     
    if(this->collisionHandler != nullptr) {
        this->collisionHandler->addCollider(obj->collider);      
    }
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
