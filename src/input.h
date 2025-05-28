#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <queue>
#include <iostream>
class InputHandler {
    
    public:
    InputHandler();
    bool key_down[80];     
    std::unordered_map<std::string,std::function<void()>> actions; 
    void registerAction(const std::string& key, std::function<void()> action);
    void executeAction(const std::string& key);
    void executeAction(int key); 
    std::unordered_map<int,std::string> keyNames  = 
{   
    {73, "A"},
    {74, "S"},
    {75, "W"},
    {76, "x"},
    {77, "d"},

};


};
