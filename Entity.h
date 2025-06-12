#ifndef ENTITY_H
#define ENTITY_H

#include "String.h"
#include <iostream>
#include <fstream>
using namespace std;

// Base Entity class - demonstrates inheritance hierarchy
class Entity {
protected:
    String name;
    int x, y;  // Position coordinates
    int id;
    bool isActive;

public:
    Entity() : x(0), y(0), id(0), isActive(true) {
        name = "Unknown";
    }
    
    Entity(const String& entityName, int posX, int posY, int entityId) 
        : name(entityName), x(posX), y(posY), id(entityId), isActive(true) {}
    
    virtual ~Entity() {}
    
    // Pure virtual function for polymorphism
    virtual void display() const = 0;
    virtual bool interact() = 0;
    
    // Getters and Setters
    String getName() const { return name; }
    void setName(const String& newName) { name = newName; }
    int getX() const { return x; }
    int getY() const { return y; }
    void setPosition(int newX, int newY) { x = newX; y = newY; }
    int getId() const { return id; }
    bool getActive() const { return isActive; }
    void setActive(bool active) { isActive = active; }
    
    // Virtual functions for polymorphism
    virtual void serialize(ofstream& file) const {
        int nameLen = name.size();
        file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        for(int i = 0; i < nameLen; i++) {
            file.write(&name[i], sizeof(char));
        }
        file.write(reinterpret_cast<const char*>(&x), sizeof(x));
        file.write(reinterpret_cast<const char*>(&y), sizeof(y));
        file.write(reinterpret_cast<const char*>(&id), sizeof(id));
        file.write(reinterpret_cast<const char*>(&isActive), sizeof(isActive));
    }
    
    virtual void deserialize(ifstream& file) {
        int nameLen;
        file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        name.clear();
        for(int i = 0; i < nameLen; i++) {
            char ch;
            file.read(&ch, sizeof(char));
            name.push_back(ch);
        }
        file.read(reinterpret_cast<char*>(&x), sizeof(x));
        file.read(reinterpret_cast<char*>(&y), sizeof(y));
        file.read(reinterpret_cast<char*>(&id), sizeof(id));
        file.read(reinterpret_cast<char*>(&isActive), sizeof(isActive));
    }
    
    // Operator overloading
    bool operator==(const Entity& other) const {
        return id == other.id;
    }
    
    friend ostream& operator<<(ostream& out, const Entity& entity) {
        out << "Entity: " << entity.name << " at (" << entity.x << ", " << entity.y << ")";
        return out;
    }
};

#endif