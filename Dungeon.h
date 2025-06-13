#ifndef DUNGEON_H
#define DUNGEON_H

#include "Location.h"
#include "Character.h"
#include "String.h"
#include <vector>

template<typename T>
class GameContainer {
private:
    vector<T*> items;
    
public:
    void add(T* item) {
        items.push_back(item);
    }
    
    T* find(int id) {
        for(T* item : items) {
            if(item->getId() == id) {
                return item;
            }
        }
        return nullptr;
    }
    
    bool remove(int id) {
        for(auto it = items.begin(); it != items.end(); ++it) {
            if((*it)->getId() == id) {
                delete *it;
                items.erase(it);
                return true;
            }
        }
        return false;
    }
    
    const vector<T*>& getAll() const {
        return items;
    }
    
    int size() const {
        return items.size();
    }
    
    void clear() {
        for(T* item : items) {
            delete item;
        }
        items.clear();
    }
    
    ~GameContainer() {
        clear();
    }
};

class Dungeon {
private:
    int width, height;
    vector<vector<Location*>> grid; // 2D array of locations - Composition
    Character* player; // Association - Dungeon knows about player
    GameContainer<Item> allItems; // Template usage
    GameContainer<Monster> allMonsters; // Template usage
    String dungeonName;
    int nextId;
    
public:
    Dungeon(int w = 10, int h = 10, const String& name = "Mysterious Dungeon") 
        : width(w), height(h), dungeonName(name), nextId(1000) {
        
        // Initialize grid
        grid.resize(height);
        for(int i = 0; i < height; i++) {
            grid[i].resize(width);
            for(int j = 0; j < width; j++) {
                grid[i][j] = new Location(j, i, LocationType::EMPTY);
            }
        }
        
        player = nullptr;
        generateDungeon();
    }
    
    ~Dungeon() {
        // Clean up grid
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                delete grid[i][j];
            }
        }
        // Note: player is not deleted here as it's managed externally
    }
    
    void generateDungeon() {
        // Create entrance
        grid[0][0]->setType(LocationType::ENTRANCE);
        grid[0][0]->setDescription("The entrance to the dungeon. You can see light from outside.");
        
        // Create exit
        grid[height-1][width-1]->setType(LocationType::EXIT);
        grid[height-1][width-1]->setDescription("The exit! Freedom awaits beyond this door.");
        
        // Create some rooms and corridors
        for(int i = 1; i < height-1; i++) {
            for(int j = 1; j < width-1; j++) {
                if((i + j) % 3 == 0) {
                    grid[i][j]->setType(LocationType::ROOM);
                    grid[i][j]->setDescription("A dimly lit room with stone walls.");
                } else if((i + j) % 5 == 0) {
                    grid[i][j]->setType(LocationType::TREASURE_ROOM);
                    grid[i][j]->setDescription("A treasure room with golden gleams in the darkness.");
                } else {
                    grid[i][j]->setType(LocationType::CORRIDOR);
                    grid[i][j]->setDescription("A narrow corridor with echoing footsteps.");
                }
            }
        }
        
        // Add some items
        addItem(new Item("Health Potion", 2, 2, nextId++, ItemType::HEALTH_POTION, 50));
        addItem(new Item("Magic Sword", 3, 4, nextId++, ItemType::SWORD, 25, false));
        addItem(new Item("Mana Potion", 5, 5, nextId++, ItemType::MANA_POTION, 30));
        addItem(new Item("Shield", 6, 3, nextId++, ItemType::SHIELD, 15, false));
        addItem(new Item("Dungeon Key", 7, 7, nextId++, ItemType::KEY, 1, false));
        
        // Add some monsters
        addMonster(new Monster("Goblin", 4, 4, nextId++, MonsterType::GOBLIN, 80, 15, 5, "Magic Sword"));
        addMonster(new Monster("Orc Warrior", 6, 6, nextId++, MonsterType::ORC, 120, 20, 8, "Shield"));
        addMonster(new Monster("Ancient Dragon", 8, 8, nextId++, MonsterType::DRAGON, 200, 35, 15, "Dungeon Key"));
    }
    
    void addItem(Item* item) {
        if(item && isValidPosition(item->getX(), item->getY())) {
            allItems.add(item);
            grid[item->getY()][item->getX()]->addItem(item);
        }
    }
    
    void addMonster(Monster* monster) {
        if(monster && isValidPosition(monster->getX(), monster->getY())) {
            allMonsters.add(monster);
            grid[monster->getY()][monster->getX()]->addMonster(monster);
        }
    }
    
    bool isValidPosition(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
    
    Location* getLocation(int x, int y) const {
        if(isValidPosition(x, y)) {
            return grid[y][x];
        }
        return nullptr;
    }
    
    void setPlayer(Character* p) {
        player = p;
    }
    
    Character* getPlayer() const {
        return player;
    }
    
    void displayMap() const {
        cout << "\n=== " << dungeonName << " Map ===" << endl;
        cout << "Legend: S=Start, E=Exit, R=Room, T=Treasure, M=Monster, I=Item, .=Corridor, ?=Unexplored" << endl;
        
        // Display player position
        if(player) {
            cout << "Player position: (" << player->getX() << ", " << player->getY() << ")" << endl;
        }
        
        cout << "\n  ";
        for(int j = 0; j < width; j++) {
            cout << j % 10;
        }
        cout << endl;
        
        for(int i = 0; i < height; i++) {
            cout << i % 10 << " ";
            for(int j = 0; j < width; j++) {
                char symbol = grid[i][j]->getMapSymbol();
                
                // Show player position
                if(player && player->getX() == j && player->getY() == i) {
                    cout << "P";
                } else {
                    cout << symbol;
                }
            }
            cout << endl;
        }
        cout << endl;
    }
    
    void displayCurrentLocation() const {
        if(player) {
            Location* loc = getLocation(player->getX(), player->getY());
            if(loc) {
                loc->enter();
            }
        }
    }
    
    bool movePlayer(char direction) {
        if(!player) return false;
        
        int newX = player->getX();
        int newY = player->getY();
        
        switch(direction) {
            case 'n': case 'N': newY--; break;
            case 's': case 'S': newY++; break;
            case 'e': case 'E': newX++; break;
            case 'w': case 'W': newX--; break;
            default:
                cout << "Invalid direction! Use N/S/E/W" << endl;
                return false;
        }
        
        if(!isValidPosition(newX, newY)) {
            cout << "Cannot move there - out of bounds!" << endl;
            return false;
        }
        
        Location* newLoc = getLocation(newX, newY);
        if(!newLoc->canAccess()) {
            cout << "Cannot access this location!" << endl;
            return false;
        }
        
        player->move(newX, newY);
        newLoc->enter();
        return true;
    }
    
    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    String getName() const { return dungeonName; }
    const GameContainer<Item>& getAllItems() const { return allItems; }
    const GameContainer<Monster>& getAllMonsters() const { return allMonsters; }
    
    // Check win condition
    bool isWinCondition() const {
        if(!player) return false;
        return (player->getX() == width-1 && player->getY() == height-1);
    }
    
    // Serialization
    void serialize(ofstream& file) const {
        file.write(reinterpret_cast<const char*>(&width), sizeof(width));
        file.write(reinterpret_cast<const char*>(&height), sizeof(height));
        file.write(reinterpret_cast<const char*>(&nextId), sizeof(nextId));
        
        int nameLen = dungeonName.size();
        file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        for(int i = 0; i < nameLen; i++) {
            file.write(&dungeonName[i], sizeof(char));
        }
        
        // Serialize grid
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                grid[i][j]->serialize(file);
            }
        }
        
        // Serialize items
        int itemCount = allItems.size();
        file.write(reinterpret_cast<const char*>(&itemCount), sizeof(itemCount));
        for(const Item* item : allItems.getAll()) {
            item->serialize(file);
        }
        
        // Serialize monsters
        int monsterCount = allMonsters.size();
        file.write(reinterpret_cast<const char*>(&monsterCount), sizeof(monsterCount));
        for(const Monster* monster : allMonsters.getAll()) {
            monster->serialize(file);
        }
    }
    
    void deserialize(ifstream& file) {
        // Clean up existing data
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                delete grid[i][j];
            }
        }
        allItems.clear();
        allMonsters.clear();
        
        file.read(reinterpret_cast<char*>(&width), sizeof(width));
        file.read(reinterpret_cast<char*>(&height), sizeof(height));
        file.read(reinterpret_cast<char*>(&nextId), sizeof(nextId));
        
        int nameLen;
        file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        dungeonName.clear();
        for(int i = 0; i < nameLen; i++) {
            char ch;
            file.read(&ch, sizeof(char));
            dungeonName.push_back(ch);
        }
        
        // Deserialize grid
        grid.resize(height);
        for(int i = 0; i < height; i++) {
            grid[i].resize(width);
            for(int j = 0; j < width; j++) {
                grid[i][j] = new Location();
                grid[i][j]->deserialize(file);
            }
        }
        
        // Deserialize items
        int itemCount;
        file.read(reinterpret_cast<char*>(&itemCount), sizeof(itemCount));
        for(int i = 0; i < itemCount; i++) {
            Item* item = new Item();
            item->deserialize(file);
            allItems.add(item);
        }
        
        // Deserialize monsters
        int monsterCount;
        file.read(reinterpret_cast<char*>(&monsterCount), sizeof(monsterCount));
        for(int i = 0; i < monsterCount; i++) {
            Monster* monster = new Monster();
            monster->deserialize(file);
            allMonsters.add(monster);
        }
    }
};

#endif