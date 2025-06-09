#include <memory>
#include <random>
#include <vector>
#include "renderer.h"
#include "shapes.h"
#include <unordered_set>
#include <queue>
#include <functional>
#include "transform.h"
class CollisionShape {
public:
    CollisionShape();
    Transform2D* transform = nullptr;
    std::string name;
    virtual bool intersects(CollisionShape& other) = 0;
    //void updatePosition(Vector2D newPosition);
};

class RectCollisionShape : public CollisionShape{
    public: 
        RectCollisionShape(); 
        bool intersects(CollisionShape& other) override; 
};

class CircleCollisionShape : public CollisionShape {
    public:
        CircleCollisionShape();
        float radius = 20.0f;
        bool intersects(CollisionShape& other) override;
};
class GameObject;
class Collider {
    
    public: 
        CollisionShape* shape = nullptr;
        bool collision(Collider& other);
        bool getCollisionStatus();
        void setCollisionStatus(bool newState);
        void resetCollisionStatus();
        std::function<void()> onCollision;
    private:
        bool _hasCollided = false;
};

class Collisionhandler {
    public:
    std::vector<Collider*> colliders;
    void addCollider(Collider* c);
    void checkCollisions();
};

class GameObject {
    public:    
        GameObject();
        GameObject(Vector2D pos);
        ~GameObject() {}         
        Transform2D* transform;
        Vector2D velocity{0.0f,0.0f}; // should be part of a physics class (like rigid body) for final engine,so don'lt couple to heavily  
        Graphic* graphic = nullptr;
        Collider* collider = new Collider();
        void move(Vector2D offset); 
        void applyVelocity();
};

class Bubble : GameObject {
    public:
        uint32_t color;
        std::vector<Bubble*> connections;
        Bubble(uint32_t colorC) : color(colorC) {}
};

enum GameObjectType {
    GameObjectArrow,
    GameObjectBubble,
    GameObjectBoundary
};

class GameObjectFactory {
    public:
        GameObject createGameObject();
        class::Arrow createArrow();
};

class GameHandler {
    public: 
        GameHandler();
        Renderer* mainRender = nullptr;
        Collisionhandler *collisionHandler = nullptr; 
        GraphicsFactory *factory = nullptr;
        std::vector<std::unique_ptr<GameObject>> gameObjects; 
        void addGameobject(/*blueprint here*/);
        GameObject* addGameobjectAt(Vector2D position); 
        GameObject* addArrow();  
        void updateObjects();
};
