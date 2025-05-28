#include "input.h"


InputHandler::InputHandler() {
}

void InputHandler::registerAction(const std::string& key, std::function<void()>  action) {
   actions[key] = action; 
}

void InputHandler::executeAction(const std::string& key) {
    auto result = actions.find(key);
    if(result != actions.end()) {
        result->second();
    }
}

void InputHandler::executeAction(int key) {
    std::cout << key << std::endl;
    if(this->keyNames.count(key) > 0)  
    executeAction(keyNames[key]);
}
