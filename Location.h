#ifndef LOCATION_H
#define LOCATION_H

#include "String.h"
#include "Item.h"
#include "Monster.h"
#include <vector>
#include <iostream>
using namespace std;

enum class LocationType {
    EMPTY,
    ROOM,
    CORRIDOR,
    TREASURE_ROOM,
    MONSTER_LAIR,
    ENTRANCE,
    EXIT
};

class Location {
private:
    int x, y;
    LocationType type;
    String description;
    bool isVisited;
    bool isAccessible;
    vector<Item*> items; // Aggregation - Location has Items
    vector<Monster*> monsters; // Aggregation - Location has Monsters
    
public:
    Location() : x(0), y(0), type(LocationType::EMPTY), isVisited(false), isAccessible(true) {
        description = "An empty space";
    }
    
    Location(int posX, int posY, LocationType locType, const String& desc = "A mysterious place")
        : x(posX), y(posY), type(locType), description(desc), isVisited(false), isAccessible(true) {}
    
    ~Location() {
        // Note: We don't delete items and monsters here as they might be managed elsewhere
        // This demonstrates different ownership patterns
    }
    
    // Display location information
    void display() const {
        cout << "=== Location (" << x << ", " << y << ") ===" << endl;
        cout << description << endl;
        
        if(!items.empty()) {
            cout << "Items here:" << endl;
            for(const Item* item : items) {
                if(item->getActive()) {
                    cout << "  - " << item->getName() << ": " << item->getDescription() << endl;
                }
            }
        }
        
        if(!monsters.empty()) {
            cout << "Creatures here:" << endl;
            for(const Monster* monster : monsters) {
                if(monster->getActive() && !monster->getDefeated()) {
                    cout << "  - " << monster->getName() << " (Hostile)" << endl;
                }
            }
        }
        
        cout << "Visited: " << (isVisited ? "Yes" : "No") << endl;
    }
    
    // Item management
    void addItem(Item* item) {
        if(item) {
            items.push_back(item);
            item->setPosition(x, y);
        }
    }
    
    bool removeItem(int itemId) {
        for(auto it = items.begin(); it != items.end(); ++it) {
            if((*it)->getId() == itemId) {
                items.erase(it);
                return true;
            }
        }
        return false;
    }
    
    Item* getItem(int itemId) {
        for(Item* item : items) {
            if(item->getId() == itemId && item->getActive()) {
                return item;
            }
        }
        return nullptr;
    }
    
    // Monster management
    void addMonster(Monster* monster) {
        if(monster) {
            monsters.push_back(monster);
            monster->setPosition(x, y);
        }
    }
    
    bool removeMonster(int monsterId) {
        for(auto it = monsters.begin(); it != monsters.end(); ++it) {
            if((*it)->getId() == monsterId) {
                monsters.erase(it);
                return true;
            }
        }
        return false;
    }
    
    Monster* getAliveMonster() {
        for(Monster* monster : monsters) {
            if(monster->getActive() && !monster->getDefeated()) {
                return monster;
            }
        }
        return nullptr;
    }
    
    // Location interaction
    void enter() {
        if(!isVisited) {
            cout << "You enter a new area..." << endl;
            isVisited = true;
        }
        display();
    }
    
    bool canAccess() const {
        return isAccessible;
    }
    
    void setAccessible(bool accessible) {
        isAccessible = accessible;
    }
    
    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    LocationType getType() const { return type; }
    String getDescription() const { return description; }
    bool getVisited() const { return isVisited; }
    const vector<Item*>& getItems() const { return items; }
    const vector<Monster*>& getMonsters() const { return monsters; }
    
    // Setters
    void setType(LocationType newType) { type = newType; }
    void setDescription(const String& desc) { description = desc; }
    void setVisited(bool visited) { isVisited = visited; }
    
    // Check if location has specific items or monsters
    bool hasItems() const {
        for(const Item* item : items) {
            if(item->getActive()) return true;
        }
        return false;
    }
    
    bool hasMonsters() const {
        for(const Monster* monster : monsters) {
            if(monster->getActive() && !monster->getDefeated()) return true;
        }
        return false;
    }
    
    // Get character representation for map
    char getMapSymbol() const {
        if(!isVisited) return '?';
        
        if(hasMonsters()) return 'M';
        if(hasItems()) return 'I';
        
        switch(type) {
            case LocationType::ENTRANCE: return 'S';
            case LocationType::EXIT: return 'E';
            case LocationType::TREASURE_ROOM: return 'T';
            case LocationType::MONSTER_LAIR: return 'L';
            case LocationType::ROOM: return 'R';
            case LocationType::CORRIDOR: return '.';
            default: return ' ';
        }
    }
    
    // Serialization
    void serialize(ofstream& file) const {
        file.write(reinterpret_cast<const char*>(&x), sizeof(x));
        file.write(reinterpret_cast<const char*>(&y), sizeof(y));
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        file.write(reinterpret_cast<const char*>(&isVisited), sizeof(isVisited));
        file.write(reinterpret_cast<const char*>(&isAccessible), sizeof(isAccessible));
        
        int descLen = description.size();
        file.write(reinterpret_cast<const char*>(&descLen), sizeof(descLen));
        for(int i = 0; i < descLen; i++) {
            file.write(&description[i], sizeof(char));
        }
        
        int itemCount = items.size();
        file.write(reinterpret_cast<const char*>(&itemCount), sizeof(itemCount));
        for(const Item* item : items) {
            item->serialize(file);
        }
        
        int monsterCount = monsters.size();
        file.write(reinterpret_cast<const char*>(&monsterCount), sizeof(monsterCount));
        for(const Monster* monster : monsters) {
            monster->serialize(file);
        }
    }
    
    void deserialize(ifstream& file) {
        file.read(reinterpret_cast<char*>(&x), sizeof(x));
        file.read(reinterpret_cast<char*>(&y), sizeof(y));
        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        file.read(reinterpret_cast<char*>(&isVisited), sizeof(isVisited));
        file.read(reinterpret_cast<char*>(&isAccessible), sizeof(isAccessible));
        
        int descLen;
        file.read(reinterpret_cast<char*>(&descLen), sizeof(descLen));
        description.clear();
        for(int i = 0; i < descLen; i++) {
            char ch;
            file.read(&ch, sizeof(char));
            description.push_back(ch);
        }
        
        int itemCount;
        file.read(reinterpret_cast<char*>(&itemCount), sizeof(itemCount));
        items.clear();
        for(int i = 0; i < itemCount; i++) {
            Item* item = new Item();
            item->deserialize(file);
            items.push_back(item);
        }
        
        int monsterCount;
        file.read(reinterpret_cast<char*>(&monsterCount), sizeof(monsterCount));
        monsters.clear();
        for(int i = 0; i < monsterCount; i++) {
            Monster* monster = new Monster();
            monster->deserialize(file);
            monsters.push_back(monster);
        }
    }
};

#endif