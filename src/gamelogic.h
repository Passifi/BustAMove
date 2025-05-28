#include <memory>
#include <random>
#include <vector>
#include "renderer.h"
#include "shapes.h"
#include <unordered_set>
#include <queue>
// collisionDetection
// Playfield 
//
// build a recipe builder
// goes through 
//
class CollisionShape {
public:
    CollisionShape();
    float x,y;
    std::string name;
    virtual bool intersects(CollisionShape& other) = 0;
    //void updatePosition(Vector2D newPosition);
};

class RectCollisionShape : public CollisionShape{
    public: 
        RectCollisionShape(); 
        float width,height;
        
        bool intersects(CollisionShape& other) override; 
};
class Collider {
    
    public: 

    CollisionShape* shape = nullptr;
    bool collision(Collider& other);
};

class Collisionhandler {


};

class GameObject {
public:    
    GameObject();
    GameObject(Vector2D pos);
    Vector2D  position{0.f,0.f}; 
    Graphic* graphic = nullptr;
    Collider* collider = new Collider();
    void move(Vector2D offset); 
};

class GameObjectFactory {
    GameObject createGameObject();
};

class GameHandler {
public: 
    GameHandler();
    Renderer* mainRender = nullptr;
    GraphicsFactory *factory = nullptr;
    std::vector<GameObject> gameObjects; 
    void addGameobject(/*blueprint here*/);
    void addGameobjectAt(Vector2D position); 
    void updateObjects();

};
       
